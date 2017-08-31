/*
 * npcsprite.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include "npcsprite.h"

#include "engine/debug/assert.h"
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


bool NPCSprite::NPCSprite::init(const Engine::Graphics::SpritePtr& sprite)
	{
	assert(!!sprite);

	this->sprite=sprite;
	}

void NPCSprite::update(float dt)
	{
	sprite.update(dt);

	NPC::update(dt);
	}

void NPCSprite::print(float)
	{
	Engine::Render::getInstance().draw(Engine::Render::getInstance().getCurrentCamera()->getBillboard(orientation.getPosition()), sprite);
	}
