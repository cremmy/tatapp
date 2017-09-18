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
	if(!model.load(definition))
		{
		return false;
		}

	return true;
	}


void NPCModel::update(float dt)
	{
	//model.update(dt); // lolz

	//orientation.rotateRight(22.5f*dt);
	//orientation.rotateUp(45.0f*dt);

	NPC::update(dt);
	}

void NPCModel::print(float)
	{
	using namespace Engine;
	using namespace Engine::Math;

	Render::getInstance().draw(orientation, model);
	}

void NPCModel::clear()
	{
	model.clear();
	}
