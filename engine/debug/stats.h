/*
 * stats.h
 *
 *  Created on: 31 lip 2017
 *      Author: crm
 */

#pragma once

#include <fstream>
#include <string>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>

namespace Engine
	{
	namespace Debug
		{
		class Stats
			{
			protected:
				SDL_Thread* thread;
				SDL_mutex* mutex;

				bool stop;

				unsigned delay;

				std::string path;
				unsigned outdelay;

			public:
				Stats(): thread(nullptr), mutex(nullptr), stop(false), delay(0u), path(""), outdelay(0u) {}
				~Stats() {}

				bool init(unsigned delay=50u);
				void clear();

				// Czas pomiedzy kolejnymi wypisaniami, w milisekundach
				void setOutput(const std::string& path, unsigned delay=250u);
			};

		} /* namespace Debug */
	} /* namespace Engine */
