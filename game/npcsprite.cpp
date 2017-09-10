/*
 * npcsprite.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include "npcsprite.h"

#include "engine/render/render.h"

using namespace Game;


NPCSprite::NPCSprite(): NPC(), sprite(nullptr)
	{
	//
	}

NPCSprite::~NPCSprite()
	{
	sprite=nullptr;
	}


bool NPCSprite::load(const std::string& definition)
	{
	sprite=Engine::Graphics::SpritePtr(definition);

	if(!sprite)
		{
		return false;
		}

	return true;
	}


void NPCSprite::update(float dt)
	{
	sprite.update(dt);

	NPC::update(dt);
	}

void NPCSprite::print(float)
	{
	using namespace Engine;
	using namespace Engine::Math;

	Render::getInstance().draw(Orientation(orientation.getPosition(), Render::getInstance().getCurrentCamera()->getBillboard(orientation.getPosition()), orientation.getScale()), sprite);
	//Render::getInstance().draw(orientation, sprite);
	}

void NPCSprite::clear()
	{
	sprite=nullptr;
	}
