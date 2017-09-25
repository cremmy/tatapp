/*
 * ingamemenu.cpp
 *
 *  Created on: 23 wrz 2017
 *      Author: crm
 */

#include "ingamemenu.h"

#include "engine/render/render.h"
#include "engine/sound/soundplayer.h"

using namespace Game::State;

enum class Actions
	{
	NONE,

	RETURN,
	HELP,
	CREDITS,
//	VOLUME_SOUND,
//	VOLUME_MUSIC,
	EXIT,
	EXIT_NO,
	EXIT_YES,
	};

IngameMenu::IngameMenu()
	{
	//
	}

IngameMenu::~IngameMenu()
	{
	//
	}


bool IngameMenu::init(Engine::Core::Application *application)
	{
	this->application=application;

	menu.init(Engine::Render::getInstance().getFrameBufferWidth(), Engine::Render::getInstance().getFrameBufferHeight(), "font/dejavu.xml");
	menu.getRootLayer()->addOption( new Menu::Option((int)Actions::RETURN, Menu::Type::RETURN, "Powr\xF3t") );
	menu.getRootLayer()->addOption( new Menu::Option((int)Actions::HELP, Menu::Type::ACTION, "Pomoc") );
	menu.getRootLayer()->addOption( new Menu::Option((int)Actions::CREDITS, Menu::Type::ACTION, "Autorzy") );
//	menu.getRootLayer()->addOption( new Menu::Option((int)Actions::VOLUME_SOUND, Menu::Type::VALUE, "Dźwięk") );
//		menu.getOptionByTag((int)Actions::VOLUME_SOUND)->initValue(0.0f, 100.0f, 1.0f);
//		//menu.getOptionByTag((int)Actions::VOLUME_SOUND)->setVal(Sound::getGlobalSoundVolume()*100.0f);
//	menu.getRootLayer()->addOption( new Menu::Option((int)Actions::VOLUME_SOUND, Menu::Type::VALUE, "Muzyka") );
//		menu.getOptionByTag((int)Actions::VOLUME_SOUND)->initValue(0.0f, 100.0f, 1.0f);
//		//menu.getOptionByTag((int)Actions::VOLUME_SOUND)->setVal(Sound::getGlobalSoundVolume()*100.0f);
	menu.getRootLayer()->addOption( new Menu::Option((int)Actions::EXIT, Menu::Type::ENTER, "Wyj\x9C""cie") );
	menu.getOptionByTag((int)Actions::EXIT)->initSubLayer( new Menu::Layer());
	menu.getOptionByTag((int)Actions::EXIT)->getSubLayer()->addOption( new Menu::Option((int)Actions::EXIT_NO, Menu::Type::RETURN, "Powr\xF3t"));
	menu.getOptionByTag((int)Actions::EXIT)->getSubLayer()->addOption( new Menu::Option((int)Actions::EXIT_YES, Menu::Type::ACTION, "Wyj\x9C""cie"));


	camUI.GUI(
		Engine::Render::getInstance().getFrameBufferWidth(),
		Engine::Render::getInstance().getFrameBufferHeight());

	background=Engine::Graphics::ImagePtr("image/fade.png");

	resume();

	return true;
	}

bool IngameMenu::update(float dt)
	{
	menu.update(dt);

	if(menu.isExited())
		{
		application->popState();
		return false;
		}

	if(menu.isBlocked())
		{
		switch(menu.getLayer()->getOption()->getTag())
			{
			case (int)Actions::RETURN:
				application->popState(); // Ryzykowne // XXX
				return false;
			break;

			case (int)Actions::HELP:
				menu.unlock();
			break;

			case (int)Actions::CREDITS:
				menu.unlock();
			break;

//			case Actions::VOLUME_SOUND:
//				menu.unlock();
//			break;
//
//			case Actions::VOLUME_MUSIC:
//				menu.unlock();
//			break;

			default:
			case (int)Actions::EXIT_YES:
				application->stop(); // To tez moze byc ryzykowne
				//return false; // Bez przesady
			break;
			}
		}

	Engine::Core::AppEvent e;
	while(popEvent(e))
		{
		if(e.data.keyboard.key==SDLK_UP || e.data.keyboard.key==SDLK_w)
			{
			menu.prev();
			}
		else if(e.data.keyboard.key==SDLK_DOWN || e.data.keyboard.key==SDLK_s)
			{
			menu.next();
			}
		else if(e.data.keyboard.key==SDLK_RETURN || e.data.keyboard.key==SDLK_d)
			{
			menu.down();
			}
		else if(e.data.keyboard.key==SDLK_ESCAPE || e.data.keyboard.key==SDLK_a)
			{
			menu.up();
			}
		}

	return false;
	}

void IngameMenu::print(float /*tinterp*/)
	{
	using namespace Engine::Math;

	Engine::Render::getInstance().statePush();
	Engine::Render::getInstance().setRenderMode(Engine::Render::RenderMode::GUI);
	Engine::Render::getInstance().setColor(AVector(1, 1, 1, 0.9));
/*	Engine::Render::getInstance().drawPolygon({
		AVector(RW*0.25f, 0),
		AVector(RW*0.25f, RH),
		AVector(RW*0.75f, RH),
		AVector(RW*0.75f, 0)
		}, AVector(0.8, 0.8, 0.8, 1), AVector(0.8, 0.8, 0.8, 1));*/
	Engine::Render::getInstance().draw(Orientation::GUI, background);
	Engine::Render::getInstance().setColor(AVector(1, 1, 1, 1));
	menu.getText().print(Orientation::GUI);
	Engine::Render::getInstance().statePop();

	//return true;
	}

void IngameMenu::clear()
	{
	menu.clear();
	application->removeListener(*this);
	}

void IngameMenu::pause()
	{
	application->removeListener(*this);
	}

void IngameMenu::resume()
	{
	application->addListener(Engine::Core::AppEvent::Type::KEY_UP, *this);
	//application->setGrabMouse(true);
	}

