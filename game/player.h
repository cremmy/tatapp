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

#include "database.h"
#include "dialog.h"
#include "fade.h"
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

			Database db;
			Fade fade;

			Engine::Math::Geometry::AABB collider;
			float eyeHeight;

			Engine::Math::AVector direction;

			float dialogCooldown;
			Dialog dialog;

			float stepTimer;
			int stepCounter;

			float orientationTargetPercent; // 0->1
			float orientationTargetTime;   // Ile sekund do konca animacji
			Engine::Math::Orientation orientationStart;
			Engine::Math::Orientation orientationTarget;

			Engine::Core::Application* application;

			void handleEvents();
			void updateMove(Engine::Math::AVector spddt);

		public:
			Player(Engine::Core::Application* application);
			virtual ~Player();

			void init(Level* lvl, const Engine::Math::Geometry::AABB& collider, float eyeHeight);
			void update(float dt);
			void print(float tinterp);

			bool isMovementFinished() const;
			const Engine::Math::Orientation& getOrientation() const {return orientation;}
			const Engine::Math::Orientation getEyeOrientation() const {return orientation+Engine::Math::AVector(0, 0, eyeHeight);}
			const NPC* getNPCTarget() const {return npcTarget;}
			Dialog& getDialog() {return dialog;}
			Database& getDatabase() {return db;}
			Fade& getFade() {return fade;}

			void setPosition(const Engine::Math::AVector& s) {orientation.setPosition(s);}
			void setMovement(const Engine::Math::Orientation& target, float time);
			void setLookAt(const Engine::Math::AVector& target, float time);
		};

	} /* namespace Game */
