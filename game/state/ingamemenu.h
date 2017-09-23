/*
 * ingamemenu.h
 *
 *  Created on: 23 wrz 2017
 *      Author: crm
 */

#pragma once

#include "../menu.h"

#include "engine/base/applicationstate.h"
#include "engine/core/appeventlistener.h"
#include "engine/graphics/imageptr.h"
#include "engine/render/camera.h"

namespace Game
	{
	namespace State
		{
		class IngameMenu: public Engine::Base::ApplicationState, public Engine::Core::AppEventListener
			{
			protected:
				Menu menu;
				Engine::Render::Camera camUI;
				Engine::Graphics::ImagePtr background;

			public:
				IngameMenu();
				virtual ~IngameMenu();

				virtual bool init(Engine::Core::Application *application);
				virtual bool update(float dt);
				virtual bool print(float tinterp);
				virtual void clear();
				virtual void pause();
				virtual void resume();

				virtual std::string getName() const {return "IngameMenu";}
			};

		} /* namespace State */
	} /* namespace Game */
