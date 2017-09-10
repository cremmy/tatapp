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

#include "engine/math/geometry/ray.h"

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
			void update(float dt);
			void print(float tinterp);
			void clear();

			NPC* findByRay(const Engine::Math::Geometry::Ray& ray);
		};

	} /* namespace Game */
