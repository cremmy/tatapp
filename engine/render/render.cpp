/*
 * render.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "render.h"

#include <cstring>
#include <sstream>

#include "textureptr.h"
#include "vertex.h"
#include "../consts.h"
#include "../debug/log.h"
#include "../graphics/bitmapptr.h"

#define GL_ERROR() \
	{\
	GLenum err=glGetError();\
	if(err!=GL_NO_ERROR)\
		{\
		LOG_ERROR("Render.%s: Blad: %s", __FUNCTION__, gluErrorString(err));\
		}\
	}

using namespace Engine::Render;

void __attribute__((__stdcall__)) debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
	std::stringstream ss;

	if(severity==GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

	switch(source)
		{
		case GL_DEBUG_SOURCE_API:             ss << "api"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   ss << "window"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "shader"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     ss << "3rd party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     ss << "app"; break;
		case GL_DEBUG_SOURCE_OTHER:           ss << "other"; break;
		}

	switch(severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			LOG_ERROR("OpenGL.debugCallback: %s: %s", ss.str().c_str(), message);
		break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			LOG_WARNING("OpenGL.debugCallback: %s: %s", ss.str().c_str(), message);
		break;

		case GL_DEBUG_SEVERITY_LOW:
		default:
			LOG_INFO("OpenGL.debugCallback: %s: %s", ss.str().c_str(), message);
		break;
		}
	}

Render* Render::instance;

Render::Render(): window(nullptr), context(nullptr), windowmode(FullScreenMode::WINDOWED), vsync(true),
	vboid(0u), uboid(0u), baseFBO(), baseShaderImage(nullptr), baseShaderPrimitive(nullptr),
	shaderPost(nullptr), textureLastIdx(0u), textures(), states()
	{
	states.reserve(8);
	}

Render::~Render()
	{
	clear();
	}

bool Render::init(int w, int h, const std::string& title, const std::string& iconpath, FullScreenMode mode)
	{
	if(window)
		{
		LOG_WARNING("Render.init: Powtorna inicjalizacja rendera");
		return true;
		}

	LOG_INFO("Render.init: Startowanie rendera...");
	LOG_DEBUG("Render.init: [%dx%dpx][title \"%s\"][icon \"%s\"][mode %s]", w, h, title.c_str(), iconpath.c_str(),
			(mode==FullScreenMode::WINDOWED)?"okno":(mode==FullScreenMode::WINDOWED_FULLSCREEN)?"pełny ekran w oknie":"pełny ekran");

	//clear();

	LOG_DEBUG("Render.init: Sterowniki:");
	for(int i=0; i<SDL_GetNumVideoDrivers(); ++i)
		{
		LOG_DEBUG("Render.init:     [%2d]: %s", i, SDL_GetVideoDriver(i));
		}

	SDL_DisplayMode dm;

	LOG_DEBUG("Render.init: Urzadzenia:");
	for(int i=0; i<SDL_GetNumVideoDisplays(); ++i)
		{
		SDL_GetCurrentDisplayMode(i, &dm);
		LOG_DEBUG("Render.init:     [%2d]: %s, %dx%dpx@%dHz", i, SDL_GetDisplayName(i), dm.w, dm.h, dm.refresh_rate);
		}

	Uint32 fullscreenflag=0u;

	switch(mode)
		{
		case FullScreenMode::WINDOWED_FULLSCREEN:
			fullscreenflag=SDL_WINDOW_FULLSCREEN_DESKTOP;
		break;

		case FullScreenMode::FULLSCREEN:
			fullscreenflag=SDL_WINDOW_FULLSCREEN;
		break;

		default:
		case FullScreenMode::WINDOWED:
			//
		break;
		}

	/**** Okno ****/
	window=SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, fullscreenflag|SDL_WINDOW_OPENGL|SDL_WINDOW_HIDDEN);

	if(!window)
		{
		LOG_ERROR("Render.init: Nie udalo sie utworzyc okna: %s", SDL_GetError());
		return false;
		}

	LOG_SUCCESS("Render.init: Utworzono okno");
	LOG_DEBUG("Render.init: Wybrany sterownik: %s", SDL_GetCurrentVideoDriver());

	SDL_GetWindowDisplayMode(window, &dm);
	LOG_INFO("Render.init: Okno: %dx%dpx@%dHz", dm.w, dm.h, dm.refresh_rate);

	if(!iconpath.empty())
		{
		icon=Graphics::BitmapPtr(iconpath);
		if(icon)
			SDL_SetWindowIcon(window, icon->getSurface());
		}

	SDL_ShowWindow(window);

	/**** OpenGL ****/
	LOG_INFO("Render.init: Tworzenie kontekstu OpenGLa...");

	context=SDL_GL_CreateContext(window);

	if(!context)
		{
		LOG_ERROR("Render.init: Nie udalo sie utworzyc kontekstu OpenGLa: %s", gluErrorString(glGetError()));
		return false;
		}

	LOG_SUCCESS("Render.init: Utworzono kontenst OpenGLa");

	/**** Info ****/
	LOG_INFO("Render.init: Info:");
	LOG_INFO("Render.init:     Producent: %s", glGetString(GL_VENDOR));
	LOG_INFO("Render.init:     GPU      : %s", glGetString(GL_RENDERER));
	LOG_INFO("Render.init:     OpenGL   : %s", glGetString(GL_VERSION));
	LOG_INFO("Render.init:     Shader   : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	LOG_INFO("Render.init: OpenGL:");
	GLint gettmp;

	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &gettmp);
	LOG_INFO("Render.init:     Maksymalna liczba tekstur: %d (min 48)", gettmp);
	textures.resize(gettmp);
	for(auto& id: textures)
		id=0u;

	glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &gettmp);
	LOG_INFO("Render.init:     Maksymalna wielkosc tekstury: %d (min 1024)", gettmp);

	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &gettmp);
	LOG_INFO("Render.init:     Maksymalna liczba uniformow: %d (min 1024)", gettmp);

	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &gettmp);
	LOG_INFO("Render.init:     Maksymalna liczba buforow uniformow: %d", gettmp);

	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &gettmp);
	LOG_INFO("Render.init:     Maksymalna wielkosc bloku uniformow: %d", gettmp);

	LOG_INFO("Render.init: Inicjalizacja GLEWa...");
	GLenum err=glewInit();

	if(err!=GL_NO_ERROR)
		{
		LOG_ERROR("Render.init: Nie udalo sie zainicjalizowac GLEWa: %s", glewGetErrorString(err));
		return false;
		}

	LOG_SUCCESS("Render.init: Zainicjalizowano GLEWa");

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, nullptr);

	/**** Bufory ****/
	LOG_INFO("Render.init: Inicjalizacja buforow...");

	glGenBuffers(1, &vboid);

	if(!vboid)
		{
		LOG_ERROR("Render.init: Nie udalo sie zainicjowac VBO [GLid: %u]", vboid);
		GL_ERROR();
		return false;
		}

	LOG_SUCCESS("Render.init: Udalo sie zainicjowac VBO");

	glGenBuffers(1, &uboid);

	if(!uboid)
		{
		LOG_ERROR("Render.init: Nie udalo sie zainicjowac UBO [GLid: %u]", uboid);
		GL_ERROR();
		return false;
		}

	LOG_SUCCESS("Render.init: Udalo sie zainicjowac UBO");

	/**** Bazowe ****/
	if(!baseFBO.init(w, h, FrameBuffer::FBO_RENDERBUFFER))
		{
		LOG_ERROR("Render.init: Nie udalo sie zainicjowac buforu ekranu");
		return false;
		}

	baseShaderImage=ShaderPtr(RENDER_DEFAULT_SHADER_IMAGE);

	if(!baseShaderImage)
		{
		LOG_ERROR("Render.init: Nie udalo sie wczytac domyslnego shadera (%s)", RENDER_DEFAULT_SHADER_IMAGE);
		return false;
		}

	baseShaderPrimitive=ShaderPtr(RENDER_DEFAULT_SHADER_PRIMITIVE);

	if(!baseShaderPrimitive)
		{
		LOG_ERROR("Render.init: Nie udalo sie wczytac domyslnego shadera prymitywow (%s)", RENDER_DEFAULT_SHADER_PRIMITIVE);
		return false;
		}

	/**** vSync ****/
	if(SDL_GL_SetSwapInterval(1)!=0)
		{
		LOG_WARNING("Render.init: Nie udalo sie wlaczyc vsynca");
		}

	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
	glClearColor(1, 1, 1, 0);
	//glClear();

//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	states.push_back(State());

	glBindBuffer(GL_UNIFORM_BUFFER, uboid);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraInfo), &states.back().caminfo, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0u);
	GL_ERROR();

	//unsetFrameBuffer();
	setFrameBuffer(baseFBO, true);
	setShader(baseShaderImage);

	return true;
	}

void Render::update()
	{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0u);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, baseFBO.getFBO());
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

//	const unsigned T0=SDL_GetTicks();
	if(shaderPost)
		{
		const Vertex data[]=
			{
			{-1, -1, 0,   0, 0},
			{-1,  1, 0,   0, 1},
			{ 1,  1, 0,   1, 1},
			{ 1, -1, 0,   1, 0}
			};

		glBindBuffer(GL_ARRAY_BUFFER, vboid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

		setShader(shaderPost);
		shaderPost.setUniforms();

		glUniform1i(shaderPost->getUniform("time"), SDL_GetTicks());

		bind(0u, baseFBO.getColorBuffer());
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		}
	else if(windowmode!=FullScreenMode::WINDOWED)
		{
		int bw, bh;
		baseFBO.getSize(bw, bh);

		int sx=0;
		int sy=0;
		int sw, sh;
		getWindowSize(sw, sh);

		const float ASPECT=bw/(float)bh;
		const float SCALE_W=sw/bw;
		const float SCALE_H=sh/bh;

		// 1920x1200 vs 1024x768
		// SCALE_W = 1.8750
		// SCALE_H = 1.5625
		if(SCALE_W>=SCALE_H)
			{
			// Czarne paski po bokach
			//int nh=sh;              // 1200
			const int nw=ceil(sh*ASPECT); // 1600
			sx=(sw-nw)/2;
			sy=0;
			sw=nw;
			//sh=nh;
			}
		else
			{
			// Czarne paski u gory i dolu
			//int nw=sw;
			const int nh=floor(sw/ASPECT);
			sx=0;
			sy=(sh-nh)/2;
			//sw=nw;
			sh=nh;
			}

		glBlitFramebuffer(0, 0, bw, bh, sx, sy, sw, sh,  GL_COLOR_BUFFER_BIT, GL_LINEAR); // <- blitowanie depth buffera powoduje INVALID_OPERATION
		}
	else
		{
		int bw, bh;
		baseFBO.getSize(bw, bh);

		int sw, sh;
		getWindowSize(sw, sh);

		glBlitFramebuffer(0, 0, bw, bh, 0, 0, bw, bh,  GL_COLOR_BUFFER_BIT, GL_NEAREST);
		}

//	const unsigned T1=SDL_GetTicks();

	SDL_GL_SwapWindow(window);

//	const unsigned T2=SDL_GetTicks();

	//LOG_INFO("blit %u swap %u", T1-T0, T2-T1);

	states.clear();
	states.push_back(State());

	unsetShader();
	//unsetFrameBuffer();
	unsetColor();
	setFrameBuffer(baseFBO, true);
	}

void Render::clear()
	{
	if(vboid)
		glDeleteBuffers(1, &vboid);

	vboid=0u;

	if(uboid)
		glDeleteBuffers(1, &uboid);

	uboid=0u;

	baseFBO.clear();

	baseShaderImage=nullptr;
	baseShaderPrimitive=nullptr;

	shaderPost=nullptr;

	textureLastIdx=0u;
	textures.clear();
	states.clear();

	if(context)
		SDL_GL_DeleteContext(context);

	context=nullptr;

	icon=nullptr;

	if(window)
		SDL_DestroyWindow(window);

	window=nullptr;
	}


void Render::getWindowSize(int& w, int& h) const
	{
	SDL_GetWindowSize(window, &w, &h);
	}

int Render::getWindowWidth() const
	{
	int w, h;
	getWindowSize(w, h);

	return w;
	}

int Render::getWindowHeight() const
	{
	int w, h;
	getWindowSize(w, h);

	return h;
	}

void Render::getFrameBufferSize(int& w, int& h) const
	{
	w=baseFBO.getWidth();
	h=baseFBO.getHeight();
	}

int Render::getFrameBufferWidth() const
	{
	return baseFBO.getWidth();
	}

int Render::getFrameBufferHeight() const
	{
	return baseFBO.getHeight();
	}

int Render::getMaxTextureSize() const
	{
	int gettmp;
	glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &gettmp);

	return gettmp;
	}

const Camera* Render::getCurrentCamera() const
	{
	const State& state=states.back();

	return state.camera;
	}

bool Render::setResolution(int w, int h, FullScreenMode mode)
	{
	LOG_INFO("Render.setResolution: Zmiana rozdzielczości na %dx%dpx", w, h);

	SDL_SetWindowSize(window, w, h);

	return setFullscreen(mode);
	}

bool Render::setFullscreen(FullScreenMode mode)
	{
	if(windowmode==mode)
		return true;

	LOG_INFO("Render.setFullscreen: Przełączanie na %s", (mode==FullScreenMode::WINDOWED)?"okno":(mode==FullScreenMode::WINDOWED_FULLSCREEN)?"pełny ekran w oknie":"pełny ekran");

	Uint32 fullscreenflag=0u;

	switch(mode)
		{
		case FullScreenMode::WINDOWED_FULLSCREEN:
			fullscreenflag=SDL_WINDOW_FULLSCREEN_DESKTOP;
		break;

		case FullScreenMode::FULLSCREEN:
			fullscreenflag=SDL_WINDOW_FULLSCREEN;
		break;

		default:
		case FullScreenMode::WINDOWED:
			//
		break;
		}

	if(SDL_SetWindowFullscreen(window, fullscreenflag)!=0)
		{
		LOG_ERROR("Render.setFullscreen: Nie udalo sie przelaczyc z/na pelny ekran");

		return false;
		}

	return true;
	}

bool Render::toggleFullscreen()
	{
	if(windowmode==FullScreenMode::FULLSCREEN)
		{
		return setFullscreen(FullScreenMode::WINDOWED);
		}
	else if(windowmode==FullScreenMode::WINDOWED)
		{
		return setFullscreen(FullScreenMode::WINDOWED_FULLSCREEN);
		}
	else //if(windowmode==FullScreenMode::WINDOWED_FULLSCREEN)
		{
		return setFullscreen(FullScreenMode::WINDOWED);
		}
	}


void Render::statePush()
	{
	states.push_back(State(states.back()));
	}

void Render::statePop()
	{
	if(states.size()<=1u)
		{
		return;
		}

	State& state=states[states.size()-2u];

	// Kamera
	glBindBuffer(GL_UNIFORM_BUFFER, uboid);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraInfo), &states.back().caminfo, GL_DYNAMIC_DRAW);

	// FBO
	//if(state.fbo) // Zawsze musi być Król Li-- framebuffer
		{
		setFrameBuffer(*state.fbo, false);
		}

	// Shader
	//if(state.shader) // Jak wyżej
		{
		setShader(state.shader);
		}

	states.pop_back();
	}


void Render::bind(unsigned idx, const TexturePtr& texture)
	{
	bind(idx, texture->getTextureID());
	}

void Render::bind(unsigned idx, GLuint tid)
	{
	if(textureLastIdx!=idx)
		{
		textureLastIdx=idx;
		glActiveTexture(GL_TEXTURE0+idx);
		}

	if(tid!=textures[idx])
		{
		textures[idx]=tid;
		glBindTexture(GL_TEXTURE_2D, tid);
		}
	}

void Render::unbind(const TexturePtr& texture)
	{
	unbind(texture->getTextureID());
	}

void Render::unbind(GLuint tid)
	{
	for(auto& btid: textures)
		{
		if(btid==tid)
			btid=0u;
		}
	}

void Render::setCamera(Camera& camera)
	{
	State& state=states.back();

	const Math::AMatrix& mv=camera.getView();
	const Math::AMatrix& mp=camera.getProjection();

	// Widok
	state.caminfo.view[ 0]=mv[0][0];
	state.caminfo.view[ 1]=mv[1][0];
	state.caminfo.view[ 2]=mv[2][0];
	state.caminfo.view[ 3]=mv[3][0];
	state.caminfo.view[ 4]=mv[0][1];
	state.caminfo.view[ 5]=mv[1][1];
	state.caminfo.view[ 6]=mv[2][1];
	state.caminfo.view[ 7]=mv[3][1];
	state.caminfo.view[ 8]=mv[0][2];
	state.caminfo.view[ 9]=mv[1][2];
	state.caminfo.view[10]=mv[2][2];
	state.caminfo.view[11]=mv[3][2];
	state.caminfo.view[12]=mv[0][3];
	state.caminfo.view[13]=mv[1][3];
	state.caminfo.view[14]=mv[2][3];
	state.caminfo.view[15]=mv[3][3];

	// Projekcja
	state.caminfo.projection[ 0]=mp[0][0];
	state.caminfo.projection[ 1]=mp[1][0];
	state.caminfo.projection[ 2]=mp[2][0];
	state.caminfo.projection[ 3]=mp[3][0];
	state.caminfo.projection[ 4]=mp[0][1];
	state.caminfo.projection[ 5]=mp[1][1];
	state.caminfo.projection[ 6]=mp[2][1];
	state.caminfo.projection[ 7]=mp[3][1];
	state.caminfo.projection[ 8]=mp[0][2];
	state.caminfo.projection[ 9]=mp[1][2];
	state.caminfo.projection[10]=mp[2][2];
	state.caminfo.projection[11]=mp[3][2];
	state.caminfo.projection[12]=mp[0][3];
	state.caminfo.projection[13]=mp[1][3];
	state.caminfo.projection[14]=mp[2][3];
	state.caminfo.projection[15]=mp[3][3];

	glBindBuffer(GL_UNIFORM_BUFFER, uboid);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraInfo), &state.caminfo, GL_DYNAMIC_DRAW);
//	GLvoid* p=glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
//	memcpy(p, &states.back().caminfo, sizeof(CameraInfo));
//	glUnmapBuffer(GL_UNIFORM_BUFFER);

	state.camera=&camera;
	}

void Render::moveCamera(const Math::AVector& move)
	{
	State& state=states.back();

//	state.caminfo.view[12]-=move.x;
//	state.caminfo.view[13]-=move.y;
//	state.caminfo.view[14]-=move.z;
	state.caminfo.view[12]-=move.x*state.caminfo.view[0]+move.y*state.caminfo.view[4]+move.z*state.caminfo.view[ 8];
	state.caminfo.view[13]-=move.x*state.caminfo.view[1]+move.y*state.caminfo.view[5]+move.z*state.caminfo.view[ 9];
	state.caminfo.view[14]-=move.x*state.caminfo.view[2]+move.y*state.caminfo.view[6]+move.z*state.caminfo.view[10];

	glBindBuffer(GL_UNIFORM_BUFFER, uboid);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraInfo), &states.back().caminfo, GL_DYNAMIC_DRAW);
	}


void Render::setFrameBuffer(FrameBuffer& fbo, bool clear)
	{
	State& state=states.back();

	if(state.fbo!=&fbo)
		{
		state.fbo=&fbo;

		glBindFramebuffer(GL_FRAMEBUFFER, fbo.getFBO());
		glViewport(0, 0, fbo.getWidth(), fbo.getHeight());
		}

	if(clear)
		{
		//glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		}
	}

void Render::unsetFrameBuffer()
	{
	setFrameBuffer(baseFBO, false);
	}

void Render::setShader(const ShaderPtr& shader)
	{
	setShader((const Shader*)shader);

	shader.setUniforms();
	}

void Render::setShader(const Shader* shader)
	{
	State& state=states.back();

	if(state.shader==shader)
		return;

	state.shader=shader;

	glUseProgram(shader->getProgramID());

	GLuint ubidx=shader->getUniformBlock(SHADER_UNIFORM_BLOCK);

	if(ubidx!=GL_INVALID_INDEX)
		{
		glUniformBlockBinding(shader->getProgramID(), ubidx, 0u); // 0u -> jak wyżej
		glBindBufferBase(GL_UNIFORM_BUFFER, 0u, uboid); // 0u -> indeks na którym bindowane jest UBO
		}
	}

void Render::unsetShader()
	{
	setShader(baseShaderImage);
	}


void Render::setShaderEffect(const ShaderPtr& shader)
	{
	if(!shader)
		return;

	shaderPost=shader;
	}

void Render::unsetShaderEffect()
	{
	shaderPost=nullptr;
	}
