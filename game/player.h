/*
 * player.h
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#pragma once

#include "engine/core/appeventlistener.h"
#include "engine/core/application.h"
#include "engine/math/orientation.h"
#include "engine/math/geometry/aabb.h"

#include "dialog.h"
#include "level.h"

namespace Game
	{
	class Player: public Engine::Core::AppEventListener
		{
		protected:
			Engine::Math::Orientation orientation;
			Engine::Math::AVector speed;

			Level* lvl;
			NPC* npcTarget;

			Engine::Math::Geometry::AABB collider;
			float eyeHeight;

			Engine::Math::AVector direction;

			float dialogCooldown;
			Dialog dialog;

			Engine::Core::Application* application;

			void handleEvents();
			void updateMove(Engine::Math::AVector spddt);

		public:
			Player(Engine::Core::Application* application);
			virtual ~Player();

			void init(Level* lvl, const Engine::Math::Geometry::AABB& collider, float eyeHeight);
			void update(float dt);
			void print(float tinterp);

			const Engine::Math::Orientation& getOrientation() const {return orientation;}
			const Engine::Math::Orientation getEyeOrientation() const {return orientation+Engine::Math::AVector(0, 0, eyeHeight);}
			const NPC* getNPCTarget() const {return npcTarget;}
			Dialog getDialog() {return dialog;}
			const Dialog& getDialog() const {return dialog;}

		};

	} /* namespace Game */
