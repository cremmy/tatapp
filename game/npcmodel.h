/*
 * npcmodel.h
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#pragma once

#include "npc.h"

#include "engine/render/model.h"

namespace Game
	{
	class NPCModel: public NPC
		{
		protected:
			Engine::Render::Model model;

			virtual bool load(const std::string& definition);

		public:
			NPCModel();
			virtual ~NPCModel();

			virtual void update(float dt);
			virtual void print(float tinterp);
			virtual void clear();
		};

	} /* namespace Game */
