/*
 * application.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <vector>

#include "appevent.h"

#include "../debug/assert.h"

namespace Engine
	{
	namespace Base
		{
		class ApplicationState;
		}

	namespace Core
		{
		class AppEventListener;

		// Stos stanów aplikacji, gra definiuje swoje specyficzne stany (dziedziczenie po ApplicationState)
		class Application
			{
			protected:
				std::vector<Base::ApplicationState*> states;
				std::map<AppEvent::Type, std::vector<AppEventListener*>> listeners;

				bool running;

				float updatetime;
				float printtime;
				float fps;

				void pushEvent(const AppEvent& e);

			public:
				Application();
				~Application();

				bool init();
				void run();
				void clear();

				bool pushState(Base::ApplicationState* state);
				bool popState();

				void addListener(AppEvent::Type type, AppEventListener& listener)
					{
#ifdef BUILD_DEBUG
					if(listeners.find(type)!=listeners.end())
						{
						for(auto* l: listeners[type])
							{
							assert(l!=&listener);
							}
						}
#endif

					listeners[type].push_back(&listener);
					}

				void removeListener(AppEventListener& listener)
					{
					for(auto itt=listeners.begin(); itt!=listeners.end(); ++itt)
						{
						for(auto itl=itt->second.begin(); itl!=itt->second.end(); ++itl)
							{
							if(*itl==&listener)
								continue;

							itt->second.erase(itl--);
							}
						}
					}

				void stop();

				float getUpdateTime() const {return updatetime;}
				float getPrintTime() const {return printtime;}
				float getFrameRate() const {return fps;}

				void setGrabMouse(bool s);
			};

		} /* namespace Core */
	} /* namespace Engine */
