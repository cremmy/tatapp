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
	class Level;
	class Player;

	class Dialog
		{
		public:
			enum class Mode
				{
				NONE,
				DIALOG,
				SELECTION,
				BACKLOG,
				};

		protected:
			Engine::IO::INI script;

			Mode mode;

			bool ready;

			int b;
			int index;

			float waitTimer;

			std::vector<int> selectionTarget;
			std::vector<std::string> selectionOption;
			int selectionIndex;

			std::string message;

			std::vector<std::string> log;
			int logIndex;

			Level* lvl;
			Player* player;

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
			Mode getMode() const {return (logIndex>0)?Mode::BACKLOG:mode;}
			std::string getMessage() const {return message;}

			void setLevel(Level* s) {lvl=s;}
			void setPlayer(Player* s) {player=s;}
		};

	} /* namespace Game */
