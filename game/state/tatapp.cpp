/*
 * game.cpp
 *
 *  Created on: 10 wrz 2017
 *      Author: crm
 */

#include "tatapp.h"
#include "../level.h"
#include "../player.h"

#include "engine/debug/log.h"
#include "engine/render/render.h"
#include "engine/graphics/ui/text.h"

#include "engine/math/orientation.h" // XXX Debug, wywalic
#include "engine/graphics/imageptr.h" // XXX Debug, wywalic

using namespace Game;
using namespace Game::State;

Engine::Graphics::ImagePtr grid;
Engine::Graphics::UI::Text text;

TATAPP::TATAPP(): Engine::Base::ApplicationState(), lvl(nullptr), player(nullptr)
	{
	cam.perspective(
		Engine::Render::getInstance().getWindowWidth(),
		Engine::Render::getInstance().getWindowHeight(),
		0.25f, 1000.0f);

	grid=Engine::Graphics::ImagePtr("image/grid_gray.png"); // XXX Debug, wywalic
	text.init("font/font_00a.xml", "Kasztanowa artyleria!\nBAM BAM BAM!\n\n\t...i nie ostal sie nikt.\n\t\t\t~Paulo Coelho", 800, 100);
	text.setAlignBottom();
	}

TATAPP::~TATAPP()
	{
	grid=nullptr; // XXX Debug, wywalic
	text.clear(); // XXX Debug, wywalic
	}

bool TATAPP::init(Engine::Core::Application *application)
	{
	LOG_INFO("TATAPP.init: Startowanie gry...");

	// Poziom
	lvl=new Level();

	if(!lvl->init("level/first.xml"))
		{
		LOG_ERROR("TATAPP.init: Nie udalo sie wczytac poziomu");
		return false;
		}

	// Gracz
	player=new Player(application);

	player->init(lvl, Engine::Math::Geometry::AABB(Engine::Math::AVector(0, 0, 0), Engine::Math::AVector(0.5f, 0.5f, 1.8f)), 1.7f);

	// Przechwytywanie myszy
	application->setGrabMouse(true);

	LOG_SUCCESS("TATAPP.init: Gra uruchomiona");

	return true;
	}

bool TATAPP::update(float dt)
	{
	lvl->update(dt);
	player->update(dt);

	cam.setOrientation(player->getEyeOrientation());
	text.update();

	return false;
	}

bool TATAPP::print(float tinterp)
	{
	using namespace Engine::Math;

	Engine::Render::getInstance().setCamera(cam);

	Engine::Render::getInstance().drawLine(AVector(0, 0, 0), AVector(2, 0, 0), AVector(1, 0, 0, 1)); // XXX Debug, wywalic
	Engine::Render::getInstance().drawLine(AVector(0, 0, 0), AVector(0, 2, 0), AVector(0, 1, 0, 1)); // XXX Debug, wywalic
	Engine::Render::getInstance().drawLine(AVector(0, 0, 0), AVector(0, 0, 2), AVector(0, 0, 1, 1)); // XXX Debug, wywalic
	Engine::Render::getInstance().draw(Orientation::FLAT_XY+AVector(-grid->getW()*0.5f, grid->getH()*0.5f, -0.125f), grid); // XXX Debug, wywalic

	lvl->print(tinterp);
	player->print(tinterp);

	text.print(Orientation(AVector(0, 0, 2), Orientation::FLAT_YZ, 0.01f));

	return false;
	}

void TATAPP::clear()
	{
	LOG_INFO("TATAPP.clear: Zwalnianie pamieci...");

	application->setGrabMouse(false);

	lvl->clear();
	delete lvl;

	//player->clear();
	delete player;

	LOG_SUCCESS("TATAPP.init: Pamiec zwolniona");
	}

void TATAPP::pause()
	{
	application->setGrabMouse(false);
	}

void TATAPP::resume()
	{
	application->setGrabMouse(true);
	}
