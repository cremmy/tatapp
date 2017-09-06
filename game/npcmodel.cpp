/*
 * npcmodel.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include "npcmodel.h"

#include "engine/render/render.h"

using namespace Game;


NPCModel::NPCModel()
	{
	//

	}

NPCModel::~NPCModel()
	{
	//
	}


bool NPCModel::load(const std::string& definition)
	{
	return false;
	}


void NPCModel::update(float dt)
	{
	NPC::update(dt);
	}

void NPCModel::print(float)
	{
	//
	}

void NPCModel::clear()
	{
	//
	}
