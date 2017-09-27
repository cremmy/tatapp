/*
 * player.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include "player.h"

#include <cmath>
#include <SDL2/SDL.h>

#include "engine/debug/log.h"
#include "engine/render/render.h"
#include "npc.h"

using namespace Game;
using namespace Engine::Math;
using namespace Engine::Math::Geometry;

const float DIALOG_COOLDOWN=0.5f;

Player::Player(Engine::Core::Application* application): Engine::Core::AppEventListener(), lvl(nullptr), npcTarget(nullptr), eyeHeight(0.0f), dialogCooldown(0.0f),
	orientationTargetPercent(1.0), orientationTargetTime(0.0f)
	{
	application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	application->addListener(Engine::Core::AppEvent::Type::KEY_UP, *this);
	application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);
	application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_UP, *this);
	application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);
	application->addListener(Engine::Core::AppEvent::Type::MOUSE_WHEEL, *this);

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

	fade.init();
	fade.addCache(Engine::Graphics::ImagePtr("image/fade.png"));
	}

void Player::update(float dt)
	{
	speed=AVector(0, 0, 0);

	handleEvents();
	fade.update(dt);

	if(dialogCooldown>0.0f)
		{
		dialogCooldown-=dt;
		}

	if(dialog.getMode()==Dialog::Mode::NONE)
		{
		if(npcTarget)
			{
			npcTarget->setTargeted();
			}

		const AVector spddt=speed*dt;
		updateMove(AVector(spddt.x, 0));
		updateMove(AVector(0, spddt.y));
		}
	else
		{
		npcTarget=nullptr;
		dialog.update(dt);
		}

	if(!isMovementFinished())
		{
		if(orientationTargetTime<0.0001f)
			{
			orientationTargetPercent=1.0f;
			}
		else
			{
			orientationTargetPercent+=dt/orientationTargetTime;

			//LOG_DEBUG("percent: %f", orientationTargetPercent);

			if(orientationTargetPercent>1.0f)
				{
				orientationTargetPercent=1.0f;
				}

			orientation=orientationStart.interpolate(orientationTarget, orientationTargetPercent);
			}
		}
	}

void Player::handleEvents()
	{
	Engine::Core::AppEvent e;

	while(popEvent(e)) // Engine::Core::EventListener::popEvent(e)
		{
		if(e.getType()==Engine::Core::AppEvent::Type::KEY_DOWN)
			{
			//
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::KEY_UP)
			{
			if(e.data.keyboard.key==SDLK_q)
				{
				LOG_INFO("Player: [p %.2f %.2f] [d %.2f %.2f]",
						orientation.getPosition().x, orientation.getPosition().y,
						orientation.getPosition().x+orientation.getForward().x, orientation.getPosition().y+orientation.getForward().y);
				}

			if(dialog.getMode()!=Dialog::Mode::NONE)
				{
				if(e.data.keyboard.key==SDLK_d)
					{
					dialog.next();
					}
				else if(e.data.keyboard.key==SDLK_a)
					{
					dialog.previous();
					}
				else if(e.data.keyboard.key==SDLK_w)
					{
					dialog.selectPrevious();
					}
				else if(e.data.keyboard.key==SDLK_s)
					{
					dialog.selectNext();
					}
				}
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_MOVE)
			{
			if(dialog.getMode()==Dialog::Mode::NONE)
				{
				// Obroty kamery
				orientation.rotateZ(e.data.mouse.x*0.2f);

				const Orientation tmpori=orientation;

				orientation.rotateRight(e.data.mouse.y*0.2f);

				const float UPDOT=orientation.getUp()|AVector(0, 0, 1);

				if(UPDOT<0.3f)
					{
					orientation=tmpori;
					}

				// Wykrywanie NPC
				npcTarget=lvl->findNPCByRay(Engine::Math::Geometry::Ray(getEyeOrientation().getPosition(), orientation.getForward()));

//				if(npcTarget)
//					{
//					LOG_INFO("Player.handleEvents: HIT: \"%s\"", npcTarget->getName().c_str());
//
//					if(!dialog.init(npcTarget->getScriptPath()))
//						{
//						LOG_ERROR("Player.handleEvents: Nie udalo sie wczytac skryptu \"%s\" NPC \"%s\"", npcTarget->getScriptPath().c_str(), npcTarget->getName().c_str());
//						}
//					else
//						{
//						LOG_INFO("Player.handleEvents: Startowanie dialogu \"%s\"", npcTarget->getScriptPath().c_str());
//						dialog.start();
//						}
//					}
//				else
//					{
//					LOG_INFO("Player.handleEvents: MISS");
//					}
				}
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN)
			{
			//
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_UP)
			{
			if(dialog.getMode()==Dialog::Mode::NONE)
				{
				if(e.data.mouse.key==1)
					{
					// Start dialogu
					if(npcTarget)
						{
						if(!dialog.init(npcTarget->getScriptPath()))
							{
							LOG_ERROR("Player.handleEvents: Nie udalo sie wczytac skryptu \"%s\" NPC \"%s\"", npcTarget->getScriptPath().c_str(), npcTarget->getName().c_str());
							}
						else if(dialogCooldown>0.0f)
							{
							LOG_INFO("Player.handleEvents: Cooldown, %fs", dialogCooldown);
							}
						else
							{
							LOG_INFO("Player.handleEvents: Startowanie dialogu \"%s\"", npcTarget->getScriptPath().c_str());
							dialog.start();
							}
						}
					}
				}
			else
				{
				if(e.data.mouse.key==1)
					{
					if(dialog.getMode()==Dialog::Mode::SELECTION)
						{
						if(dialogCooldown>0.0f)
							{
							// Czekaj
							}
						else
							{
							dialog.next();
							dialogCooldown=DIALOG_COOLDOWN;
							}
						}
					else
						{
						dialog.next();
						dialogCooldown=DIALOG_COOLDOWN;
						}
					}
				else if(e.data.mouse.key==3)
					{
					dialog.previous();
					}
				}
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_WHEEL)
			{
			if(dialog.getMode()==Dialog::Mode::SELECTION)
				{
				if(e.data.mouse.y>0)
					{
					dialog.selectPrevious();
					}
				else
					{
					dialog.selectNext();
					}
				}
			}
		}

	if(dialog.getMode()==Dialog::Mode::NONE)
		{
		const Uint8* key=SDL_GetKeyboardState(nullptr);

		const float BASE_SPEED=1.8f;

		const AVector nfwd=AVectorNormalize(AVector(orientation.getForward(), 0, 0));
		const AVector nrgt=AVectorNormalize(AVector(orientation.getRight(), 0, 0));

		if(key[SDL_SCANCODE_W]) speed+=key[SDL_SCANCODE_W]*BASE_SPEED*nfwd;
		if(key[SDL_SCANCODE_S]) speed-=key[SDL_SCANCODE_S]*BASE_SPEED*nfwd;
		if(key[SDL_SCANCODE_D]) speed+=key[SDL_SCANCODE_D]*BASE_SPEED*nrgt;
		if(key[SDL_SCANCODE_A]) speed-=key[SDL_SCANCODE_A]*BASE_SPEED*nrgt;
		}
	}

void Player::updateMove(AVector spddt)
	{
	spddt.z=0.0f;

	if(lvl->test(collider+orientation.getPosition()+spddt))
		{
		//LOG_WARNING("Aah~");
		}
	else
		{
		orientation.move(spddt);
		}
	}


void Player::print(float /*tinterp*/)
	{
	/*auto col=collider+orientation.getPosition();
	const AVector cpos=col.getPosition();
	const AVector csize=col.getHalfSize();
	const AVector cu=AVector(0, 1, 0)*csize.y;
	const AVector cr=AVector(1, 0, 0)*csize.x;
	const AVector cf=AVector(0, 0, 1)*csize.z;

	Engine::Render::getInstance().drawPolygon(
		{
		cpos+cu-cr+cf,
		cpos+cu+cr+cf,
		cpos-cu+cr+cf,
		cpos-cu-cr+cf,
		}, AVector(1, 0, 0, 1), AVector(1, 0, 0, 0.7));*/

	fade.print();
	}


bool Player::isMovementFinished() const
	{
	if(orientationTargetPercent>=1.0f)
		return true;
	return false;
	}


void Player::setMovement(const Engine::Math::Orientation& target, float time)
	{
	if(!isMovementFinished())
		{
		LOG_WARNING("NPC.serMovement: Ruch gracza jeszcze sie nie zakonczyl");
		orientation=orientationTarget;
		}

	if(time<=0.0f)
		{
		orientation=orientationTarget;
		return;
		}

	orientationStart=orientation;
	orientationTarget=target;
	orientationTargetTime=time;
	orientationTargetPercent=0.0f;
	}

void Player::setLookAt(const Engine::Math::AVector& target, float time)
	{
	if(!isMovementFinished())
		{
		LOG_WARNING("NPC.serMovement: Ruch gracza jeszcze sie nie zakonczyl");
		orientation=orientationTarget;
		}

	Engine::Math::Orientation otarget;
	otarget.lookAt(target-AVector(0, 0, eyeHeight), orientation.getPosition(), Engine::Math::AVector(0, 0, 1));

	if(time<=0.0f)
		{
		orientation=orientationTarget;
		return;
		}

	orientationStart=orientation;
	orientationTarget=otarget;
	orientationTargetTime=time;
	orientationTargetPercent=0.0f;
	}
