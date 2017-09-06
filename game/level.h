/*
 * level.h
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <string>
#include <vector>

namespace Game
	{
	class NPC;

	class Level
		{
		protected:
			std::vector<NPC*> npcs;

		public:
			Level();
			~Level();

			bool init(const std::string& path);
			void update();
			void print();
			void clear();
		};

	} /* namespace Game */
