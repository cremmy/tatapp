/*
 * player.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include "player.h"

#include <cmath>
#include <SDL2/SDL.h>

#include "npc.h"

using namespace Game;
using namespace Engine::Math;
using namespace Engine::Math::Geometry;

Player::Player(Engine::Core::Application* application): Engine::Core::AppEventListener(), lvl(nullptr), eyeHeight(0.0f)
	{
	application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	application->addListener(Engine::Core::AppEvent::Type::KEY_UP, *this);
	application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);
	application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_UP, *this);
	application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);

	this->application=application;
	}

Player::~Player()
	{
	application->removeListener(*this);
	}


void Player::init(Level* lvl, const AABB& collider, float eyeHeight)
	{
	this->lvl=lvl;
	this->collider=collider;
	this->eyeHeight=eyeHeight;

	orientation.lookAt(AVector(10, 20, -eyeHeight), AVector(0, 0, 0), AVector(0, 0, 1));
	}

void Player::update(float dt)
	{
	handleEvents();

	if(dialog.getMode()==Dialog::Mode::NONE)
		{
		updateMove(speed*dt);
		speed=AVector(0, 0, 0);
		}
	}

void Player::handleEvents()
	{
	Engine::Core::AppEvent e;

	while(popEvent(e)) // Engine::Core::EventListener::popEvent(e)
		{
		if(e.getType()==Engine::Core::AppEvent::Type::KEY_DOWN)
			{
			//direction=AVector(orientation.getForward().x, orientation.getForward().y, 0.0f);
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_MOVE)
			{
			orientation.rotateZ(e.data.mouse.x*0.2f);

			const Orientation tmpori=orientation;

			orientation.rotateRight(e.data.mouse.y*0.2f);

			const float UPDOT=orientation.getUp()|AVector(0, 0, 1);

			if(UPDOT<0.3f)
				{
				orientation=tmpori;
				}
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN)
			{
			//
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_UP)
			{
			NPC* target=lvl->findByRay(Engine::Math::Geometry::Ray(getEyeOrientation().getPosition(), orientation.getForward()));

			if(target)
				{
				LOG_INFO("Player.handleEvents: HIT: \"%s\"", target->getName().c_str());
				}
			else
				{
				LOG_INFO("Player.handleEvents: MISS");
				}
			}
		}

	const Uint8* key=SDL_GetKeyboardState(nullptr);

	const float BASE_SPEED=1.8f;

	const AVector nfwd=AVectorNormalize(AVector(orientation.getForward(), 0, 0));
	const AVector nrgt=AVectorNormalize(AVector(orientation.getRight(), 0, 0));

	if(key[SDL_SCANCODE_W]) speed+=key[SDL_SCANCODE_W]*BASE_SPEED*nfwd;
	if(key[SDL_SCANCODE_S]) speed-=key[SDL_SCANCODE_S]*BASE_SPEED*nfwd;
	if(key[SDL_SCANCODE_D]) speed+=key[SDL_SCANCODE_D]*BASE_SPEED*nrgt;
	if(key[SDL_SCANCODE_A]) speed-=key[SDL_SCANCODE_A]*BASE_SPEED*nrgt;
	}

void Player::updateMove(AVector spddt)
	{
	spddt.z=0.0f;

	orientation.move(spddt);
	}


void Player::print(float /*tinterp*/)
	{
	//
	}
