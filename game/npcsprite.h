/*
 * npcsprite.h
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#pragma once

#include "npc.h"

#include "engine/graphics/spriteptr.h"

namespace Game
	{
	class NPCSprite: public NPC
		{
		protected:
			Engine::Graphics::SpritePtr sprite;

		public:
			NPCSprite();
			virtual ~NPCSprite();

			bool init(const Engine::Graphics::SpritePtr& sprite);
			virtual void update(float dt);
			virtual void print(float tinterp);

			Engine::Graphics::SpritePtr getSprite() {return sprite;}
			const Engine::Graphics::SpritePtr getSprite() const {return sprite;}
		};

	} /* namespace Game */
