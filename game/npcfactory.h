/*
 * npcfactory.h
 *
 *  Created on: 6 wrz 2017
 *      Author: crm
 */

#pragma once

#include <string>

namespace Game
	{
	class NPC;

	NPC* createNPC(const std::string& path);
	} /* namespace Game */
