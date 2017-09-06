/*
 * debug.cpp
 *
 *  Created on: 19 cze 2017
 *      Author: crm
 */

#include "debug.h"

#include "engine/debug/log.h"
#include "engine/render/camera.h"
#include "engine/render/render.h"

#include "engine/graphics/spriteptr.h"
#include "engine/render/model.h"

#include "../level.h"

using namespace Game;
using namespace Game::State;

namespace Local
	{
	Engine::Render::Camera cam;

	Engine::Graphics::ImagePtr grid;
	Engine::Render::Model mdl;

	Level* lvl;
	}

Debug::Debug(): ApplicationState()
	{
	//
	}

Debug::~Debug()
	{
	//
	}

bool Debug::init(Engine::Core::Application *application)
	{
	using namespace Engine;
	using namespace Engine::Math;
	using namespace Local;

	LOG_DEBUG("State.Debug.init");

	this->application=application;
	this->application->addListener(Core::AppEvent::Type::KEY_DOWN, *this);
	this->application->addListener(Core::AppEvent::Type::MOUSE_MOVE, *this);
	this->application->addListener(Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);
	this->application->addListener(Core::AppEvent::Type::MOUSE_KEY_UP, *this);

	//cam.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight(), 1.0f, 1000.0f);
	cam.perspective(Render::getInstance().getWindowWidth(), Render::getInstance().getWindowHeight(), 0.1);
	//cam.lookAt(Engine::Math::AVector(0, 0, 0), Engine::Math::AVector(0, 0, 500), Engine::Math::AVector(0, 1, 0));
	cam.lookAt(AVector(0, 0, 1.7), AVector(-2, -2, 1.7));

	if(!(grid=Graphics::ImagePtr("image/grid_gray.png")))
		return false;

	if(!mdl.load("model/sample_cube.obj"))
		return false;

	lvl=new Level();

	if(!lvl->init("level/first.xml"))
		{
		return false;
		}

	return true;
	}

bool Debug::update(float dt)
	{
	using namespace Local;

	const Uint8* key=SDL_GetKeyboardState(nullptr);

	const float CAM_SPEED=1.5f;
	const float CAM_SPEED_MOD=3.0f;

	if(key[SDL_SCANCODE_W]) cam.moveForward( dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));
	if(key[SDL_SCANCODE_S]) cam.moveForward(-dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));
	if(key[SDL_SCANCODE_D]) cam.moveRight( dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));
	if(key[SDL_SCANCODE_A]) cam.moveRight(-dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));
	if(key[SDL_SCANCODE_SPACE]) cam.moveUp( dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));
	if(key[SDL_SCANCODE_LCTRL]) cam.moveUp(-dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));

	Engine::Core::AppEvent e;

	while(popEvent(e)) // Engine::Core::EventListener::popEvent(e)
		{
		if(e.getType()==Engine::Core::AppEvent::Type::KEY_DOWN)
			{
			if(e.data.keyboard.key==SDLK_1)
				{
				cam.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight());
				}
			else if(e.data.keyboard.key==SDLK_2)
				{
				cam.perspective(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight());
				}
			else if(e.data.keyboard.key==SDLK_3)
				{
				cam.lookAt(Engine::Math::AVector(0, 0, 0), Engine::Math::AVector(0, 0, 500), Engine::Math::AVector(0, 1, 0));
				}
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_MOVE && (e.data.mouse.key&SDL_BUTTON(3)))
			{
			cam.rotateZ(e.data.mouse.x*0.2f);
			cam.rotateRight(e.data.mouse.y*0.2f);
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN && e.data.mouse.key==3)
			{
			this->application->setGrabMouse(true);
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_UP && e.data.mouse.key==3)
			{
			this->application->setGrabMouse(false);
			}
		}

	return false; // nie, nie aktualizuj stanów poniżej
	}

bool Debug::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Local;

	Engine::Render::getInstance().setCamera(cam);

	Engine::Render::getInstance().drawLine(AVector(0, 0, 0), AVector(2, 0, 0), AVector(1, 0, 0, 1));
	Engine::Render::getInstance().drawLine(AVector(0, 0, 0), AVector(0, 2, 0), AVector(0, 1, 0, 1));
	Engine::Render::getInstance().drawLine(AVector(0, 0, 0), AVector(0, 0, 2), AVector(0, 0, 1, 1));
	Engine::Render::getInstance().draw(Orientation::FLAT_XY+AVector(-grid->getW()*0.5f, grid->getH()*0.5f, -0.125f), grid);

	Engine::Render::getInstance().draw(Orientation(AVector(2, 2, 1), Orientation::FLAT_XY.getRotated(AVector(0, 0, 1), SDL_GetTicks()*0.01f), 1.0f/64.0f), mdl);

	return true; // Tak, wyświetlaj stany poniżej
	}

void Debug::clear()
	{
	using namespace Local;

	LOG_DEBUG("State.Debug.clear");

	grid=nullptr;
	mdl.clear();

	delete lvl;
	}

void Debug::pause()
	{
	LOG_DEBUG("State.Debug.pause");
	}

void Debug::resume()
	{
	LOG_DEBUG("State.Debug.resume");
	}

