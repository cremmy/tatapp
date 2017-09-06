/*
 * npcfactory.cpp
 *
 *  Created on: 6 wrz 2017
 *      Author: crm
 */

#include "npcfactory.h"

#include <cstring>

#include "npcmodel.h"
#include "npcsprite.h"

#include "engine/debug/log.h"
#include "engine/io/resource.h"

using namespace Game;

NPC* Game::createNPC(const std::string& path)
	{
	NPC* npc=nullptr;

	char* data=Engine::IO::Resource::load(path);

	if(strstr(data, "sprite"))
		{
		npc=new NPCSprite();
		}
	else
		{
		npc=new NPCModel();
		}

	delete [] data;

	if(!npc->init(path))
		{
		return nullptr;
		}

	return npc;
	}
