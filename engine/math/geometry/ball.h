/*
 * circle.h
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
			class Ball: public BaseFigure, public IHasSupportPoint
				{
				protected:
					AVector position;
					float radius;

				public:
					Ball(): position(), radius(0.0f) {}
					Ball(const AVector& position, float radius): position(position), radius(radius) {}
					Ball(const Ball&)=default;
					~Ball() {}

					Ball& operator=(const Ball&)=default;

					Ball operator+(const AVector& v) const {return Ball(position+v, radius);}
					Ball operator-(const AVector& v) const {return Ball(position-v, radius);}
					Ball operator*(float s) const {return Ball(position, radius*s);}

					Ball& operator+=(const AVector& v) {position+=v; return *this;}
					Ball& operator-=(const AVector& v) {position-=v; return *this;}
					Ball& operator*=(float s) {radius*=s; return *this;}

					virtual Type getType() const {return Type::BALL;}
					virtual AVector getMinExtent() const {return position-AVector(radius, radius, radius);}
					virtual AVector getMaxExtent() const {return position+AVector(radius, radius, radius);}
					//virtual Boundary getBoundary() const {}
					virtual AVector getSupport(const AVector& dir) const {return position+dir*radius;}

					AVector getPosition() const {return position;}
					float getRadius() const {return radius;}

					void setPosition(const AVector& s) {position=s;}
					void setRadius(float s) {radius=s;}
				};

			typedef Ball Circle;
			}
		} /* namespace Math */
	} /* namespace Engine */
