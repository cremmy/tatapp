/*
 * game.cpp
 *
 *  Created on: 10 wrz 2017
 *      Author: crm
 */

#include "tatapp.h"

#include "ingamemenu.h"
#include "../level.h"
#include "../player.h"

#include "engine/debug/log.h"
#include "engine/math/orientation.h"
#include "engine/render/render.h"
#include "engine/graphics/atlas.h"
#include "engine/graphics/ui/text.h"

//#include "engine/graphics/imageptr.h" // XXX Debug, wywalic

using namespace Game;
using namespace Game::State;

const int UI_TEXT_MARGIN_H=24;
const int UI_TEXT_MARGIN_V=24;

Engine::Graphics::ImagePtr grid;

TATAPP::TATAPP(): Engine::Base::ApplicationState(), lvl(nullptr), player(nullptr), uiDialogAlpha(0.0f), uiCrosshairAlpha(0.0f)
	{
	cam.perspective(
		Engine::Render::getInstance().getFrameBufferWidth(),
		Engine::Render::getInstance().getFrameBufferHeight(),
		0.25f, 1000.0f);
//	camUI.GUI(
//		Engine::Render::getInstance().getFrameBufferWidth(),
//		Engine::Render::getInstance().getFrameBufferHeight());

//	grid=Engine::Graphics::ImagePtr("image/grid_gray.png"); // XXX Debug, wywalic
	}

TATAPP::~TATAPP()
	{
//	grid=nullptr; // XXX Debug, wywalic
	uiDialogNormal=nullptr;
	uiDialogChoice=nullptr;
	uiDialogBacklog=nullptr;
	uiDialog=nullptr;
	}

bool TATAPP::init(Engine::Core::Application *application)
	{
	LOG_INFO("TATAPP.init: Startowanie gry...");

	this->application=application;

	// Interfejs
	if(!initUI())
		{
		LOG_ERROR("TATAPP.init: Nie udalo sie wczytac grafik interfejsu");
		return false;
		}

	if(!uiText.init("font/dejavu.xml", "", uiDialogNone->getW()-2*UI_TEXT_MARGIN_H, uiDialogNone->getH()-2*UI_TEXT_MARGIN_V, -1, 4))
		{
		LOG_ERROR("TATAPP.init: Nie udalo sie wczytac czcionki");
		LOG_WARNING("TATAPP.init: Z tego co mi wiadomo, 'czcionka' to pojedynczy znak. Zestaw czcionek nazywany jest 'fontem'.");
		return false;
		}

	// Poziom
	lvl=new Level();

	if(!lvl->init("level/01.xml"))
		{
		LOG_ERROR("TATAPP.init: Nie udalo sie wczytac poziomu");
		return false;
		}

	// Gracz
	player=new Player(application);

	const float PLAYER_COLLIDER_SIZE=0.6;
	const float PLAYER_COLLIDER_HEIGHT=1.8;
	const float PLAYER_EYE_HEIGHT=1.7;
	player->init(lvl,
			Engine::Math::Geometry::AABB(
				Engine::Math::AVector(0, 0, PLAYER_COLLIDER_HEIGHT*0.5f),
				Engine::Math::AVector(PLAYER_COLLIDER_SIZE, PLAYER_COLLIDER_SIZE, PLAYER_COLLIDER_HEIGHT)),
			PLAYER_EYE_HEIGHT);

	player->getDialog().setLevel(lvl);
	player->getDialog().setPlayer(player);

	player->setPosition(Engine::Math::AVector(0, 2, 0));
	if(!player->getDialog().init("npc/start.ini"))
		{
		LOG_ERROR("TATAPP.init: Nie znaleziono skryptu startowego, aaaaaaaa");
		}
	else
		{
		player->getDialog().start();
		}

	// Oswietlenie początkowe
	Engine::Render::getInstance().setLight(
		Engine::Math::AVectorNormalize(Engine::Math::AVector(0, 1, -0.2)),
		Engine::Math::AVector(1, 1, 1));

	// Przechwytywanie myszy
	resume();
	application->setGrabMouse(true);

	LOG_SUCCESS("TATAPP.init: Gra uruchomiona");

	return true;
	}

bool TATAPP::initUI()
	{
	using namespace Engine::Graphics;

	Atlas atlas;

	if(!atlas.init(1024))
		{
		LOG_ERROR("TATAPP.initUI: Nie udalo sie wystartowac atlasu");
		return false;
		}

	if(!atlas.addImage(BitmapPtr("image/ui_dialog_none.png")) ||
	   !atlas.addImage(BitmapPtr("image/ui_dialog_normal.png")) ||
	   !atlas.addImage(BitmapPtr("image/ui_dialog_choice.png")) ||
	   !atlas.addImage(BitmapPtr("image/ui_dialog_backlog.png")) ||
	   !atlas.addImage(BitmapPtr("image/ui_crosshair.png")))
		{
		LOG_ERROR("TATAPP.initUI: Nie udalo sie dodac rysunkow do atlasu");
		return false;
		}

	if(!atlas.finalize())
		{
		return false;
		}

	uiDialogNone=atlas.getImage(0u);
	uiDialogNormal=atlas.getImage(1u);
	uiDialogChoice=atlas.getImage(2u);
	uiDialogBacklog=atlas.getImage(3u);
	uiCrosshair=atlas.getImage(4u);

	return true;
	}

bool TATAPP::update(float dt)
	{
	Engine::Core::AppEvent e;

	while(popEvent(e))
		{
		if(e.getType()!=Engine::Core::AppEvent::Type::KEY_UP)
			continue;
		if(e.data.keyboard.key!=SDLK_ESCAPE)
			continue;

		application->pushState(new IngameMenu());
		return false;
		}

	lvl->update(dt);
	player->update(dt);

	cam.setOrientation(player->getEyeOrientation());

	if(player->getDialog().getMode()!=Dialog::Mode::NONE)
		{
		if(uiCrosshairAlpha>0.0f)
			uiCrosshairAlpha-=dt;
		else
			uiCrosshairAlpha=0.0f;

		if(player->getDialog().getMessage().length()<=1)
			{
			if(uiDialogAlpha>0.0f)
				uiDialogAlpha-=dt;
			else
				uiDialogAlpha=0.0f;
			}
		else if(uiDialogAlpha<1.0f)
			uiDialogAlpha+=dt*2.0f;
		else
			uiDialogAlpha=1.0f;

		if(!player->getDialog().isReady())
			{
			uiDialog=uiDialogNone;
			}
		else if(player->getDialog().getMode()==Dialog::Mode::DIALOG)
			{
			uiDialog=uiDialogNormal;
			}
		else if(player->getDialog().getMode()==Dialog::Mode::BACKLOG)
			{
			uiDialog=uiDialogBacklog;
			}
		else
			{
			uiDialog=uiDialogChoice;
			}

		uiText=player->getDialog().getMessage();
		uiText.update();
		}
	else
		{
		uiDialog=uiDialogNone;

		if(uiCrosshairAlpha<1.0f)
			uiCrosshairAlpha+=2.0*dt;
		else
			uiCrosshairAlpha=1.0f;

		if(uiDialogAlpha>0.0f)
			uiDialogAlpha-=dt;
		else
			uiDialogAlpha=0.0f;
		}

	return false;
	}

void TATAPP::print(float tinterp)
	{
	using namespace Engine::Math;

	Engine::Render::getInstance().setCamera(cam);

//	Engine::Render::getInstance().drawLine(AVector(0, 0, 0), AVector(2, 0, 0), AVector(1, 0, 0, 1)); // XXX Debug, wywalic
//	Engine::Render::getInstance().drawLine(AVector(0, 0, 0), AVector(0, 2, 0), AVector(0, 1, 0, 1)); // XXX Debug, wywalic
//	Engine::Render::getInstance().drawLine(AVector(0, 0, 0), AVector(0, 0, 2), AVector(0, 0, 1, 1)); // XXX Debug, wywalic
//	Engine::Render::getInstance().draw(Orientation::FLAT_XY+AVector(-grid->getW()*0.5f, grid->getH()*0.5f, -0.125f), grid); // XXX Debug, wywalic

	lvl->print(tinterp);
	player->print(tinterp);

	Engine::Render::getInstance().statePush();
	Engine::Render::getInstance().setRenderMode(Engine::Render::RenderMode::GUI);

	if(uiDialogAlpha>0.0f)
		{
//		Engine::Render::getInstance().setCamera(camUI);
		Engine::Render::getInstance().setColor(AVector(1, 1, 1, uiDialogAlpha));

		const AVector UI_DIALOG_OFFSET(
			(Engine::Render::getInstance().getFrameBufferWidth()-uiDialog->getW())*0.5f,
			Engine::Render::getInstance().getFrameBufferHeight()-uiDialog->getH());

		Engine::Render::getInstance().draw(Orientation::GUI+UI_DIALOG_OFFSET, uiDialog);
		uiText.print(Orientation::GUI+UI_DIALOG_OFFSET+AVector(UI_TEXT_MARGIN_H, UI_TEXT_MARGIN_V, 0.1f));
		}

	if(uiCrosshairAlpha>0.0f)
		{
		Engine::Render::getInstance().setColor(AVector(1, 1, 1, uiCrosshairAlpha));

		const AVector UI_CROSSHAIR_OFFSET(
			(Engine::Render::getInstance().getFrameBufferWidth()+uiCrosshair->getW())*0.5f,
			(Engine::Render::getInstance().getFrameBufferHeight()+uiCrosshair->getH())*0.5f);

		Engine::Render::getInstance().draw(Orientation::GUI+UI_CROSSHAIR_OFFSET+AVector(0, 0, 0.2f), uiCrosshair);
		}
	Engine::Render::getInstance().statePop();

	//return false;
	}

void TATAPP::clear()
	{
	LOG_INFO("TATAPP.clear: Zwalnianie pamieci...");

	application->setGrabMouse(false);
	application->removeListener(*this);

	lvl->clear();
	delete lvl;

	//player->clear();
	delete player;

	LOG_SUCCESS("TATAPP.clear: Pamiec zwolniona");
	}

void TATAPP::pause()
	{
	application->setGrabMouse(false);
	application->removeListener(*this);
	}

void TATAPP::resume()
	{
	application->setGrabMouse(true);
	application->addListener(Engine::Core::AppEvent::Type::KEY_UP, *this);
	}
