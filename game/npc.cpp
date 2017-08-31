/*
 * npc.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include "npc.h"

#include "engine/debug/log.h"

using namespace Game;

NPC::NPC(): orientationTargetPercent(1.0f), orientationTargetTime(0.0f),
	name("<unnamed>"), scriptEnabled(false), scriptPath("<not set>"), visible(true),
	collidable(false)
	{
	//
	}

NPC::~NPC()
	{
	//
	}

void NPC::update(float dt)
	{
	if(isMovementFinished())
		{
		return;
		}

	if(orientationTargetTime<0.0001f)
		{
		orientationTargetPercent=1.0f;
		}
	else
		{
		orientationTargetPercent+=dt/orientationTargetTime;

		if(orientationTargetPercent>1.0f)
			{
			orientationTargetPercent=1.0f;
			}

		orientation=orientationStart.interpolate(orientationTarget, orientationTargetPercent);
		}
	}

bool NPC::isMovementFinished() const
	{
	if(orientationTargetPercent>=1.0f)
		return true;
	return false;
	}

void NPC::setMovement(const Engine::Math::Orientation& target, float time)
	{
	if(!isMovementFinished())
		{
		LOG_WARNING("NPC.serMovement: Ruch obiektu \"%s\" sie jeszcze nie zakonczyl", name.c_str());
		}

	orientationStart=orientation;
	orientationTarget=target;
	orientationTargetTime=time;
	orientationTargetPercent=0.0f;
	}
