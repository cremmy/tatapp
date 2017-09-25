/*
 * main.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include <game/state/tatapp.h>
#include <SDL2/SDL.h>

#include "engine/debug/log.h"
#include "engine/debug/stats.h"
#include "engine/core/application.h"
#include "engine/render/render.h"
#include "engine/sound/soundplayer.h"

#include "game/state/tatapp.h"

#include "engine/math/collision/test.h"

/* XXX Problemy z silnikiem:
 *  - Model przestawia shadera na swojego, ale tekstura juz tego nie robi - po wyrysowaniu modelu tekstura rysuje sie z shaderem modelowym
 */

int main(int argc, char *argv[])
	{
	if(!Engine::Debug::Log::init())
		{
		return __LINE__;
		}

	//Engine::Debug::Log::setLevel(Engine::Debug::Log::ALL);
	//Engine::Debug::Log::setPrintLocation(true);

#ifdef BUILD_RELEASE
	Engine::Debug::Log::setOut(fopen("log.txt", "w"));
	Engine::Debug::Log::setLevel(Engine::Debug::Log::PRODUCTION);
#endif

	LOG_INFO("Start");
	LOG_INFO("Build %s %s", __DATE__, __TIME__);

	//Engine::Math::Collision::_check2D();
	//Engine::Math::Collision::_check3D();

	Engine::Core::Application app;

	if(!app.init())
		{
		return __LINE__;
		}

	if(!Engine::Render::getInstance().init(1024, 768, "Tragedia Antyczna w Trzech Aktach Prozą Pisana", "image/icon.png", Engine::Render::FullScreenMode::WINDOWED))
		{
		return __LINE__;
		}

	if(!Engine::Sound::getInstance().init(32))
		{
		return __LINE__;
		}

	if(app.pushState(new Game::State::TATAPP()))
		{
		app.run();
		}

	Engine::Sound::getInstance().clear();
	Engine::Render::getInstance().clear();
	app.clear();

	LOG_INFO("Stop");

	return 0;
	}


