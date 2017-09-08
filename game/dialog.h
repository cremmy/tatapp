/*
 * dialog.h
 *
 *  Created on: 8 wrz 2017
 *      Author: crm
 */

#pragma once

#include <string>
#include <vector>

#include "engine/io/ini.h"

namespace Game
	{
	class Dialog
		{
		public:
			enum class Mode
				{
				NONE,
				DIALOG,
				SELECTION,
				};

		protected:
			Engine::IO::INI script;

			Mode mode;

			bool ready;

			int b;
			int index;

			float waitTimer;

			std::vector<int> selection;
			int selectionIndex;

			std::string message;

			std::vector<std::string> log;
			int logIndex;

		public:
			Dialog();
			~Dialog();

			bool init(const std::string& path);
			void update(float dt);

			void start();

			void next();
			void previous();

			void selectPrevious();
			void selectNext();

			bool isReady() const {return ready;}
			Mode getMode() const {return mode;}
			std::string getMessage() const {return message;}
		};

	} /* namespace Game */
