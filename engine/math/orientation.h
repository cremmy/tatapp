/*
 * orientation.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include "amath.h"
#include "aquaternion.h"

namespace Engine
	{
	namespace Math
		{
		class Orientation
			{
			protected:
				AVector position;
				AVector right;
				AVector forward;
				AVector up;
				float scale;

			public:
				Orientation(): position(), right(), forward(), up(), scale(1.0f) {} //
				Orientation(const AVector& position, const AVector& right, const AVector& up, float scale=1.0f): position(position), right(AVectorNormalize(right)), forward(AVectorNormalize(up^right)), up(AVectorNormalize(up)), scale(scale) {}
				Orientation(const AVector& position, const Orientation& orientation): position(position), right(orientation.right), forward(orientation.forward), up(orientation.up), scale(orientation.scale) {}
				Orientation(const AVector& position, const Orientation& orientation, float scale): position(position), right(orientation.right), forward(orientation.forward), up(orientation.up), scale(scale) {}
				Orientation(const Orientation& o): position(o.position), right(o.right), forward(o.forward), up(o.up), scale(o.scale) {}
				virtual ~Orientation() {}

				Orientation operator+(const AVector& v) const {return Orientation(position+v, right, up);}
				Orientation operator-(const AVector& v) const {return Orientation(position-v, right, up);}
				Orientation& operator+=(const AVector& v) {position+=v; return *this;}
				Orientation& operator-=(const AVector& v) {position-=v; return *this;}

				AVector getPosition() const {return position;}
				AVector getRight() const {return right;}
				AVector getForward() const {return forward;}
				AVector getUp() const {return up;}
				float getScale() const {return scale;}
				AMatrix getMatrix() const
					{
					return AMatrixTranspose(AMatrix(
						right*scale,
						up*scale,
						-forward*scale,
						AVector(position, 1)));/**
					AMatrix(
						AVector(1, 0, 0, position.x),
						AVector(0, 1, 0, position.y),
						AVector(0, 0, 1, position.z),
						AVector(0, 0, 0, 1)
						)*/
					}
				//AQuaternion getRotation() const {return rotation;}

				void setPosition(const AVector& pos) {position=pos;}
				void setScale(float s) {scale=s;}
				void move(const AVector& path) {position+=path;}
				void moveRight(float dist) {position+=getRight()*dist;}
				void moveForward(float dist) {position+=getForward()*dist;}
				void moveUp(float dist) {position+=getUp()*dist;}
				void rotateRight(float angle) {rotate(getRight(), angle);}
				void rotateForward(float angle) {rotate(getForward(), angle);}
				void rotateUp(float angle) {rotate(getUp(), angle);}
				void rotateX(float angle) {rotate(AVector(1.0f, 0.0f, 0.0f), angle);}
				void rotateY(float angle) {rotate(AVector(0.0f, 1.0f, 0.0f), angle);}
				void rotateZ(float angle) {rotate(AVector(0.0f, 0.0f, 1.0f), angle);}
				void rotate(const AVector& axis, float angle) {const AMatrix m=AMatrixRotate(angle, axis); right=m*right; up=m*up; forward=up^right;}

				void lookAt(const AVector& target, const AVector& from, const AVector& up=AVector(0, 0, 1));
				void lookAt(const AVector& target, float angle, float elevation, float distance, float /*roll*/=0.0f);

				Orientation interpolate(const Orientation& target, float p) const;

				Orientation getMoved(const AVector& path) const
					{
					Orientation ret(*this);
					ret.move(path);
					return ret;
					}

				Orientation getRotated(const AVector& axis, float angle) const
					{
					Orientation ret(*this);
					ret.rotate(axis, angle);
					return ret;
					}

				Orientation getScaled(float scale) const
					{
					Orientation ret(*this);
					ret.setScale(scale);
					return ret;
					}

				// todo pozostałe z gatunku getXed
				public:
					static const Orientation FLAT_XY;
					static const Orientation FLAT_XZ;
					static const Orientation FLAT_YZ;
					static const Orientation GUI;
			};

		} /* namespace Graphics */
	} /* namespace Engine */
