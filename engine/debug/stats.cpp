/*
 * stats.cpp
 *
 *  Created on: 31 lip 2017
 *      Author: crm
 */

#include "stats.h"

#define WINVER 0x0500
#include <windows.h>
#undef ERROR
#include <GL/gl.h>
#include <SDL2/SDL.h>

#include "log.h"

using namespace Engine::Debug;

bool Stats::init(unsigned delay)
	{
	LOG_INFO("Stats.init: Startowanie logera wydajnosciowego...");

	auto thfun=[](void *data)->int
		{
		Stats& base=*(Stats*)data;

		SDL_LockMutex(base.mutex);

		std::fstream out;
		out.open(base.path.c_str(), std::ios::out);

		if(!out.is_open())
			{
			LOG_ERROR("Stats.[thread]: Nie udalo sie otworzyc pliku \"%s\"", base.path.c_str());
			return 1;
			}

		const unsigned DELAY_LOOP=base.delay;
		const unsigned DELAY_OUTPUT=base.outdelay;

		unsigned lastOutputT=SDL_GetTicks();

		// Zmienne i stale
		// RAM
		MEMORYSTATUSEX memRam;
		memRam.dwLength=sizeof(memRam);

		// VRAM
		/*const unsigned GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX=0x9048;
		const unsigned GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX=0x9049;
		int memVRamTotal;
		int memVRamInit;
		int memVRamCurrent;

		glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &memVRamTotal);
		glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &memVRamInit);*/

		while(!base.stop)
			{
			SDL_UnlockMutex(base.mutex);

			// RAM
			//

			// VRAM
			/*glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &memVRamCurrent);*/

			// Wypisanie
			if(lastOutputT+DELAY_OUTPUT<SDL_GetTicks())
				{
				LOG_INFO("Stats.[thread]: RAM: %d VRAM %d", -1, /*(memVRamTotal)/1024*/-1);
				}

			SDL_Delay(DELAY_LOOP);

			SDL_LockMutex(base.mutex);
			}
		SDL_UnlockMutex(base.mutex);

		return 0;
		};

	if(delay<50u)
		delay=50u;
	else if(delay>5000u)
		delay=5000u;

	this->delay=delay;

	mutex=SDL_CreateMutex();

	if(!mutex)
		{
		LOG_ERROR("Stats.init: Nie udalo sie utworzyc muteksa");
		return false;
		}

	SDL_LockMutex(mutex);

	stop=false;

	thread=SDL_CreateThread(thfun, "statlogger", this);

	if(!thread)
		{
		LOG_ERROR("Stats.init: Nie udalo sie uruchomic watku\n");
		return false;
		}

	SDL_UnlockMutex(mutex);

	LOG_SUCCESS("Stats.init: Wystartowano logera wydajnosciowego");

	return true;
	}

void Stats::clear()
	{
	LOG_INFO("Stats.clear: Zatrzymywanie logera...");

	SDL_LockMutex(mutex);
	stop=true;
	SDL_UnlockMutex(mutex);

	SDL_WaitThread(thread, nullptr);

	LOG_INFO("Status.clear: Loger zatrzymany.");

	SDL_DestroyMutex(mutex);

	thread=nullptr;
	mutex=nullptr;
	}


void Stats::setOutput(const std::string& path, unsigned delay)
	{
	if(mutex)
		{
		SDL_LockMutex(mutex);
		}

	if(delay<50u)
		delay=50u;
	else if(delay>5000u)
		delay=5000u;

	this->outdelay=delay;
	this->path=path;

	if(mutex)
		{
		SDL_UnlockMutex(mutex);
		}
	}
