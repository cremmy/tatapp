/*
 * npc.h
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#pragma once

#include <string>

#include "engine/math/orientation.h"
#include "engine/math/geometry/aabb.h"
#include "engine/math/geometry/box.h"

namespace Game
	{
	class NPC
		{
		protected:
			static const Engine::Math::AVector MARK_COLOR;

			Engine::Math::Orientation orientation;

			float orientationTargetPercent; // 0->1
			float orientationTargetTime;   // Ile sekund do konca animacji
			Engine::Math::Orientation orientationStart;
			Engine::Math::Orientation orientationTarget;

			std::string name;

			bool scriptEnabled;
			std::string scriptPath;

			bool visible;

			bool collidable;
			Engine::Math::Geometry::AABB collider;

			bool targeted;

			virtual bool load(const std::string& definition)=0;

		public:
			NPC();
			virtual ~NPC();

			bool init(const std::string& path);
			virtual void update(float dt);
			virtual void print(float tinterp)=0;
			virtual void clear()=0;

			Engine::Math::Orientation& getOrientation() {return orientation;}
			const Engine::Math::Orientation& getOrientation() const {return orientation;}
			Engine::Math::AVector getPosition() const {return orientation.getPosition();}
			const std::string& getName() const {return name;}
			const std::string& getScriptPath() const {return scriptPath;}
			//Engine::Math::Geometry::AABB getCollider() const {return Engine::Math::Geometry::AABB(orientation.getPosition(), collider.getSize());}
			Engine::Math::Geometry::Box getCollider() const;// {return Engine::Math::Geometry::Box(orientation-orientation.getMatrix()*collider.getPosition(), collider.getSize());}

			bool isMovementFinished() const;
			bool isScriptEnabled() const {return scriptEnabled;}
			bool isVisible() const {return visible;}
			bool isCollidable() const {return collidable;}

			void setPosition(const Engine::Math::AVector& s) {orientation.setPosition(s);}
			void setOrientation(const Engine::Math::Orientation& s) {orientation=s;}
			void setMovement(const Engine::Math::Orientation& target, float time);// {orientationStart=orientation; orientationTarget=target; orientationTargetPercent=0.0f; orientationTargetTime=time;}
			void setName(const std::string& s) {name=s;}
			void setScript(const std::string& s) {scriptEnabled=true; scriptPath=s;}
			void setScriptEnabled(bool s) {scriptEnabled=s;}
			void setVisibility(bool s) {visible=s;}
			void setCollider(const Engine::Math::Geometry::AABB& s) {collidable=true; collider=s;}
			void setCollisionEnabled(bool s) {collidable=s;}
			void setTargeted() {targeted=true;}
		};

	} /* namespace Game */
