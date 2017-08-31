/*
 * line.h
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
			class Line: public BaseFigure, public IHasIndexedSupportPoint
				{
				protected:
					AVector a;
					AVector b;

				public:
					Line(): a(), b() {}
					Line(const AVector& a, const AVector& b): a(a), b(b) {}
					Line(const AVector& start, const AVector& direction, float length): a(start), b(start+direction*length) {}
					Line(const Line&)=default;
					~Line() {}

					Line& operator=(const Line&)=default;

					Line operator+(const AVector& v) const {return Line(a+v, b+v);}
					Line operator-(const AVector& v) const {return Line(a-v, b-v);}

					Line& operator+=(const AVector& v) {a+=v; b+=v; return *this;}
					Line& operator-=(const AVector& v) {a-=v; b-=v; return *this;}

					virtual Type getType() const {return Type::LINE;}
					virtual AVector getMinExtent() const {return AVectorMin(a, b);}
					virtual AVector getMaxExtent() const {return AVectorMax(a, b);}
					//virtual Boundary getBoundary() const {}

					virtual AVector getSupport(const AVector& dir) const {return ((b-a)|dir)>=0.0f?b:a;}

					virtual int getSupportID(const AVector& dir) const {return ((b-a)|dir)>=0.0f?1:0;}
					virtual AVector getSupportByID(int id) const {return id==0?a:b;}

					AVector getPositionA() const {return a;}
					AVector getPositionB() const {return b;}
				};
			}
		} /* namespace Math */
	} /* namespace Engine */
