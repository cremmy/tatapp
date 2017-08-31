/*
 * capsule.h
 *
 *  Created on: 29 lip 2017
 *      Author: crm
 */

#pragma once

#include "basefigure.h"
#include "isupportpoint.h"
#include "../amath.h"

namespace Engine
	{
	namespace Math
		{
		namespace Geometry
			{
			class Capsule: public BaseFigure, public IHasSupportPoint
				{
				protected:
					AVector a;
					AVector b;
					float radius;

				public:
					Capsule(): a(), b(), radius(0.0f) {}
					Capsule(const AVector& position, float radius): a(position), b(position), radius(radius) {}
					Capsule(const AVector& a, const AVector& b, float radius): a(a), b(b), radius(radius) {}
					Capsule(const Capsule&)=default;
					~Capsule() {}

					Capsule& operator=(const Capsule&)=default;

					Capsule operator+(const AVector& v) const {return Capsule(a+v, b+v, radius);}
					Capsule operator-(const AVector& v) const {return Capsule(a-v, b-v, radius);}

					Capsule& operator+=(const AVector& v) {a+=v; b+=v; return *this;}
					Capsule& operator-=(const AVector& v) {a-=v; b-=v; return *this;}

					virtual Type getType() const {return Type::CAPSULE;}
					virtual AVector getMinExtent() const {return AVectorMin(a, b)-AVector(radius, radius, radius);}
					virtual AVector getMaxExtent() const {return AVectorMax(a, b)+AVector(radius, radius, radius);}
					//virtual Boundary getBoundary() const {}

					virtual AVector getSupport(const AVector& dir) const {return (((b-a)|dir)>=0.0f?b:a)+dir*radius;}

					AVector getPositionA() const {return a;}
					AVector getPositionB() const {return b;}
					float getRadius() const {return radius;}
				};
			}
		} /* namespace Math */
	} /* namespace Engine */
