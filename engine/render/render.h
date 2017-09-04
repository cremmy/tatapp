/*
 * render.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */
// https://stackoverflow.com/a/39047129 // Wymuszenie zewnętrznej grafiki

#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include "camera.h"
#include "shaderptr.h"
#include "framebuffer.h"
#include "../math/orientation.h"
#include "../graphics/bitmapptr.h"

//#define RENDER Engine::Graphics::Render::Render::getInstance()

namespace Engine
	{
	namespace Graphics
		{
		class ImagePtr;
		class SpritePtr;
		}

	namespace Render
		{
		enum class FullScreenMode
			{
			WINDOWED,
			WINDOWED_FULLSCREEN,
			FULLSCREEN,
			};

		class FrameBuffer;
		class Shader;
		class Texture;
		class TexturePtr;
		class VertexBuffer;
		class Model;

		class Render
			{
			protected:
				static Render* instance;

				Render();
				~Render();

				struct CameraInfo
					{
					CameraInfo() {}
					CameraInfo(const CameraInfo& ci) {memcpy(this, &ci, sizeof(CameraInfo));}

					CameraInfo& operator=(const CameraInfo&& ci) {memcpy(this, &ci, sizeof(CameraInfo)); return *this;}

					float view[16];
					float projection[16];
					};

				struct State
					{
					State(): caminfo(), fbo(nullptr), shader(nullptr), camera(nullptr) {}
					State(const State& s): caminfo(s.caminfo), fbo(s.fbo), shader(s.shader), camera(s.camera) {}

					CameraInfo caminfo;
					FrameBuffer* fbo;
					const Shader* shader;
					const Camera* camera;
					};

				// Okno
				SDL_Window* window;
				SDL_GLContext context;

				FullScreenMode windowmode;
				bool vsync;
				Graphics::BitmapPtr icon;

				// Bufory
				GLuint vboid; // VBO dla rysowania rysunkow
				GLuint uboid; // UBO (projekcia, widok)

				// Domyślne bufory, kamery, shadery i co tam jeszcze się może pojawić
				FrameBuffer baseFBO;
				ShaderPtr baseShaderImage;
				ShaderPtr baseShaderPrimitive;

				// Postprocessing
				ShaderPtr shaderPost;

				// Stany rendera
				unsigned textureLastIdx;
				std::vector<GLuint> textures;

				std::vector<State> states;

				void setShader(const Shader* shader);

			public:
				inline static Render& getInstance()
					{
					if(!instance)
						instance=new Render();

					return *instance;
					}

				bool init(int w, int h, const std::string& title, const std::string& icon="", FullScreenMode mode=FullScreenMode::WINDOWED);
				void update();
				void clear();

				void getWindowSize(int& w, int& h) const;
				int getWindowWidth() const;
				int getWindowHeight() const;
				int getMaxTextureSize() const;
				const Camera* getCurrentCamera() const;

				bool setResolution(int w, int h) {return setResolution(w, h, windowmode);}
				bool setResolution(int w, int h, FullScreenMode mode);
				bool setFullscreen(FullScreenMode mode);

				void statePush();
				void statePop();

				inline void bind(const TexturePtr& texture) {bind(0u, texture);}
				void bind(unsigned idx, const TexturePtr& texture);
				void bind(unsigned idx, GLuint tid);
				void unbind(const TexturePtr& texture);
				void unbind(GLuint tid);

				void setCamera(Camera& camera); // Pobiera aktualną pozycję i projekcję kamery, nie przechowuje wskaźnika na nią
				void moveCamera(const Math::AVector& move);
				void setFrameBuffer(FrameBuffer& fbo, bool clear=true);
				void unsetFrameBuffer();
				void setShader(const ShaderPtr& shader);
				void unsetShader();

				void setShaderEffect(const ShaderPtr& shader);
				void unsetShaderEffect();

				/*void bind(GLuint texture);
				void bind(const Texture& tex);
				void bind(const TexturePtr& tptr);*/

				void drawLine(const Math::AVector& a, const Math::AVector& b, const Math::AVector& color=Math::AVector(1, 1, 1, 1));
				//void drawGrid(const Math::AVector& center, const Math::AVector& normal, unsigned sx, unsigned sy, const Math::AVector& color=Math::AVector(1, 1, 1, 1));
				void drawPolygon(const std::vector<Math::AVector>& vertices, const Math::AVector& color=Math::AVector(1, 1, 1, 1), const Math::AVector& fill=Math::AVector(0, 0, 0, 0));

				void draw(const Math::Orientation& orientation, const VertexBuffer& vbo);
				void draw(const Math::Orientation& orientation, const VertexBuffer& vbo, unsigned first, unsigned last);
				void draw(const Math::Orientation& orientation, const Model& mdl);
				void draw(const Math::Orientation& orientation, const Texture& tex);
				void draw(const Math::Orientation& orientation, const Texture& tex, float x, float y, float w, float h);
				void draw(const Math::Orientation& orientation, const TexturePtr& tptr);
				void draw(const Math::Orientation& orientation, const TexturePtr& tptr, float x, float y, float w, float h);
				void draw(const Math::Orientation& orientation, const Graphics::ImagePtr& iptr);
				void draw(const Math::Orientation& orientation, const Graphics::SpritePtr& sptr);
			};

		inline Render& getInstance() {return Render::getInstance();}
		} /* namespace Graphics */
	} /* namespace Engine */
