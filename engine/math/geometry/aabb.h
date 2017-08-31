/*
 * aarectangle.h
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
			class AABB: public BaseFigure, public IHasIndexedSupportPoint
				{
				protected:
					AVector position;
					AVector hsize;

				public:
					AABB(): position(), hsize() {}
					AABB(const AVector& position, float width, float depth, float height=0.0f): position(position), hsize(width*0.5f, depth*0.5f, height*0.5f) {}
					AABB(const AVector& position, const AVector& size): position(position), hsize(size*0.5f) {}
					//AABB(const AVector& a, const AVector& b): position((b+a)/2.0f), hsize((b-a)*0.5f) {}
					AABB(const AABB&)=default;
					~AABB() {}

					inline AABB& operator=(const AABB&)=default;

					inline AABB operator+(const AVector& v) const {return AABB(position+v, hsize*2.0f);}
					inline AABB operator-(const AVector& v) const {return AABB(position-v, hsize*2.0f);}
					inline AABB operator*(float s) const {return AABB(position, hsize*2.0f*s);}

					inline AABB& operator+=(const AVector& v) {position+=v; return *this;}
					inline AABB& operator-=(const AVector& v) {position-=v; return *this;}
					inline AABB& operator*=(float s) {hsize*=s; return *this;}

					inline virtual Type getType() const {return Type::AABB;}
					inline virtual AVector getMinExtent() const {return position-hsize;}
					inline virtual AVector getMaxExtent() const {return position+hsize;}
					//inline virtual Boundary getBoundary() const {return Boundary(position-hsize, position+hsize);}

					inline virtual AVector getSupport(const AVector& dir) const
						{
						if(dir.z>=0.0f)
							{
							if(dir.y>=0.0f)
								{
								if(dir.x>=0.0f)
									return position+AVector(hsize.x, hsize.y, hsize.z);
								return position+AVector(-hsize.x, hsize.y, hsize.z);
								}
							else
								{
								if(dir.x>=0.0f)
									return position+AVector(hsize.x, -hsize.y, hsize.z);
								return position+AVector(-hsize.x, -hsize.y, hsize.z);
								}
							}
						else
							{
							if(dir.y>=0.0f)
								{
								if(dir.x>=0.0f)
									return position+AVector(hsize.x, hsize.y, -hsize.z);
								return position+AVector(-hsize.x, hsize.y, -hsize.z);
								}
							else
								{
								if(dir.x>=0.0f)
									return position+AVector(hsize.x, -hsize.y, -hsize.z);
								return position+AVector(-hsize.x, -hsize.y, -hsize.z);
								}
							}
						}
					virtual int getSupportID(const AVector& dir) const
						{
						if(dir.z>=0.0f)
							{
							if(dir.y>=0.0f)
								{
								if(dir.x>=0.0f)
									return 2;
								return 1;
								}
							else
								{
								if(dir.x>=0.0f)
									return 3;
								return 0;
								}
							}
						else
							{
							if(dir.y>=0.0f)
								{
								if(dir.x>=0.0f)
									return 6;
								return 5;
								}
							else
								{
								if(dir.x>=0.0f)
									return 7;
								return 4;
								}
							}
						}
					virtual AVector getSupportByID(int id) const
						{
						switch(id)
							{
							case 0: return position+AVector(-hsize.x, -hsize.y,  hsize.z);
							case 1: return position+AVector(-hsize.x,  hsize.y,  hsize.z);
							case 2: return position+AVector( hsize.x,  hsize.y,  hsize.z);
							case 3: return position+AVector( hsize.x, -hsize.y,  hsize.z);

							case 4: return position+AVector(-hsize.x, -hsize.y, -hsize.z);
							case 5: return position+AVector(-hsize.x,  hsize.y, -hsize.z);
							case 6: return position+AVector( hsize.x,  hsize.y, -hsize.z);
							default:
							case 7: return position+AVector( hsize.x, -hsize.y, -hsize.z);
							}
						}

					inline AVector getPosition() const {return position;}
					inline float getWidth() const {return hsize.x*2.0f;}
					inline float getDepth() const {return hsize.y*2.0f;}
					inline float getHeight() const {return hsize.z*2.0f;}
					inline AVector getSize() const {return hsize*2.0f;}
					inline AVector getHalfSize() const {return hsize;}

					inline void setPosition(const AVector& s) {position=s;}
					inline void setSize(const AVector& s) {hsize=s*0.5f;}
				};
			}
		} /* namespace Math */
	} /* namespace Engine */
