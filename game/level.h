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

#include "engine/math/orientation.h"
#include "engine/math/geometry/aabb.h"
#include "engine/math/geometry/ray.h"

namespace Game
	{
	class NPC;

	class Level
		{
		protected:
			std::vector<NPC*> npcs;
			Engine::Math::Orientation startpos;

		public:
			Level();
			~Level();

			bool init(const std::string& path);
			void update(float dt);
			void print(float tinterp);
			void clear();

			NPC* findNPCByName(const std::string& name);
			NPC* findNPCByRay(const Engine::Math::Geometry::Ray& ray);
			bool test(const Engine::Math::Geometry::AABB& box);
		};

	} /* namespace Game */
