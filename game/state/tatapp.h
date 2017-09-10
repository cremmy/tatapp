/*
 * game.h
 *
 *  Created on: 10 wrz 2017
 *      Author: crm
 */

#pragma once

#include "engine/base/applicationstate.h"
#include "engine/render/camera.h"

namespace Game
	{
	class Level;
	class Player;

	namespace State
		{
		class TATAPP: public Engine::Base::ApplicationState
			{
			protected:
				Level *lvl;
				Player *player;

				Engine::Render::Camera cam;

			public:
				TATAPP();
				virtual ~TATAPP();

				virtual bool init(Engine::Core::Application *application);
				virtual bool update(float dt);
				virtual bool print(float tinterp);
				virtual void clear();
				virtual void pause();
				virtual void resume();

				//virtual int getId() const {return 0xDEB0C;};
				virtual std::string getName() const {return "Game";}
			};

		} /* namespace State */
	} /* namespace Game */
