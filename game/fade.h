/*
 * fade.h
 *
 *  Created on: 16 maj 2016
 *      Author: crm
 */

#pragma once

#include <vector>

#include "engine/graphics/imageptr.h"

namespace Game
	{
	class Fade
		{
		protected:
			std::vector<Engine::Graphics::ImagePtr> cache;

			struct Layer
				{
				Layer(): alpha(0.0f), target(0.0f), image() {}

				float alpha;
				float target;

				Engine::Graphics::ImagePtr image;
				};

			std::vector<Layer*> layers;

		public:
			Fade() {}
			virtual ~Fade() {clear();}

			bool init();
			void update(float dt);
			void print();
			void clear();

			void addCache(const Engine::Graphics::ImagePtr& image) {cache.push_back(image);}

			void set(unsigned layer, const Engine::Graphics::ImagePtr& image, float targetalpha, float alpha=-1);
			void set(unsigned layer, float targetalpha, float alpha=-1);

			float getPct(unsigned layer) const;
		};
	}
