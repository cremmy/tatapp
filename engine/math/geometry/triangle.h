/*
 * triangle.h
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
			class Triangle: public BaseFigure, public IHasIndexedSupportPoint
				{
				protected:
					AVector a;
					AVector b;
					AVector c;

				public:
					Triangle(): a(), b(), c() {}
					Triangle(const AVector& a, const AVector& b, const AVector& c): a(a), b(b), c(c) {}
					//Triangle(const AVector& center, const AVector& b, const AVector& n): a(a), b(b), c(c) {} // Konstruktor trojkata rownobocznego... potrzebne mi to w ogole?
					Triangle(const Triangle&)=default;
					~Triangle() {}

					Triangle& operator=(const Triangle&)=default;

					Triangle operator+(const AVector& v) const {return Triangle(a+v, b+v, c+v);}
					Triangle operator-(const AVector& v) const {return Triangle(a-v, b-v, c-v);}

					Triangle& operator+=(const AVector& v) {a+=v; b+=v; c+=v; return *this;}
					Triangle& operator-=(const AVector& v) {a-=v; b-=v; c-=v; return *this;}

					virtual Type getType() const {return Type::TRIANGLE;}
					virtual AVector getMinExtent() const {return AVectorMin(a, AVectorMin(b, c));}
					virtual AVector getMaxExtent() const {return AVectorMax(a, AVectorMax(b, c));}
					//virtual Boundary getBoundary() const {return Boundary(position, position);}

					virtual AVector getSupport(const AVector& dir) const
						{
						const float dota=a|dir;
						const float dotb=b|dir;
						const float dotc=c|dir;

						if(dota>dotb && dota>dotc) return a;
						if(dotb>dota && dotb>dotc) return b;
						return c;
						}

					virtual int getSupportID(const AVector& dir) const
						{
						const float dota=a|dir;
						const float dotb=b|dir;
						const float dotc=c|dir;

						if(dota>dotb && dota>dotc) return 0;
						if(dotb>dota && dotb>dotc) return 1;
						return 2;
						}

					virtual AVector getSupportByID(int id) const
						{
						switch(id)
							{
							case 0: return a;
							case 1: return b;
							default:
							case 2: return c;
							}
						}


					AVector getPositionA() const {return a;}
					AVector getPositionB() const {return b;}
					AVector getPositionC() const {return c;}
				};
			}
		} /* namespace Math */
	} /* namespace Engine */
