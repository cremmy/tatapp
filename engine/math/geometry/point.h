/*
 * point.h
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
			class Point: public BaseFigure, public IHasIndexedSupportPoint
				{
				protected:
					AVector position;

				public:
					Point(): position() {}
					Point(const AVector& position): position(position) {}
					//explicit Point(const AVector& position): position(position) {}
					Point(const Point&)=default;
					~Point() {}

					Point& operator=(const Point&)=default;
					operator AVector() const {return position;}

					Point operator+(const AVector& v) const {return Point(position+v);}
					Point operator-(const AVector& v) const {return Point(position-v);}

					Point& operator+=(const AVector& v) {position+=v; return *this;}
					Point& operator-=(const AVector& v) {position-=v; return *this;}

					virtual Type getType() const {return Type::POINT;}
					virtual AVector getMinExtent() const {return position;}
					virtual AVector getMaxExtent() const {return position;}
					//virtual Boundary getBoundary() const {return Boundary(position, position);}

					virtual AVector getSupport(const AVector&) const {return position;}

					virtual int getSupportID(const AVector& /*dir*/) const {return 0;}
					virtual AVector getSupportByID(int /*id*/) const {return position;}

					AVector getPosition() const {return position;}

					void setPosition(const AVector& s) {position=s;}
				};
			}
		} /* namespace Math */
	} /* namespace Engine */
