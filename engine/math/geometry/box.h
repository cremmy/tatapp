/*
 * box.h
 *
 *  Created on: 29 lip 2017
 *      Author: crm
 */

#pragma once

#include "basefigure.h"
#include "isupportpoint.h"
#include "../orientation.h"

namespace Engine
	{
	namespace Math
		{
		namespace Geometry
			{
			class Box: public BaseFigure, public IHasIndexedSupportPoint
				{
				protected:
					Orientation orientation;
					AVector hsize;

				public:
					Box(): orientation(), hsize() {}
					Box(const AVector& position, float width, float depth, float height=0.0f): orientation(position, Orientation::FLAT_XY), hsize(width*0.5f, depth*0.5f, height*0.5f) {}
					Box(const AVector& position, const AVector& size): orientation(position, Orientation::FLAT_XY), hsize(size*0.5f) {}
					Box(const Orientation& orientation, const AVector& size): orientation(orientation), hsize(size*0.5f) {}
					//Box(const AVector& a, const AVector& b): orientation((b+a)/2.0f), hsize((b-a)*0.5f) {}
					Box(const Box&)=default;
					~Box() {}

					Box& operator=(const Box&)=default;

					Box operator+(const AVector& v) const {return Box(orientation+v, hsize*2.0f);}
					Box operator-(const AVector& v) const {return Box(orientation-v, hsize*2.0f);}
					Box operator*(float s) const {return Box(orientation, hsize*2.0f*s);}

					Box& operator+=(const AVector& v) {orientation+=v; return *this;}
					Box& operator-=(const AVector& v) {orientation-=v; return *this;}
					Box& operator*=(float s) {hsize*=s; return *this;}

					virtual Type getType() const {return Type::BOX;}
					virtual AVector getMinExtent() const {return getSupport(AVector(-1, -1, -1));}
					virtual AVector getMaxExtent() const {return getSupport(AVector( 1,  1,  1));}
					//virtual Boundary getBoundary() const {}

					virtual AVector getSupport(const AVector& gdir) const
						{
						const AVector dir=orientation.getMatrix()*gdir;

						if(dir.z>=0.0f)
							{
							if(dir.y>=0.0f)
								{
								if(dir.x>=0.0f)
									return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(hsize.x, hsize.y, hsize.z);
								return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(-hsize.x, hsize.y, hsize.z);
								}
							else
								{
								if(dir.x>=0.0f)
									return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(hsize.x, -hsize.y, hsize.z);
								return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(-hsize.x, -hsize.y, hsize.z);
								}
							}
						else
							{
							if(dir.y>=0.0f)
								{
								if(dir.x>=0.0f)
									return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(hsize.x, hsize.y, -hsize.z);
								return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(-hsize.x, hsize.y, -hsize.z);
								}
							else
								{
								if(dir.x>=0.0f)
									return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(hsize.x, -hsize.y, -hsize.z);
								return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(-hsize.x, -hsize.y, -hsize.z);
								}
							}
						}

					virtual int getSupportID(const AVector& gdir) const
						{
						const AVector dir=orientation.getMatrix()*gdir;

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
							case 0: return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(-hsize.x, -hsize.y,  hsize.z);
							case 1: return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(-hsize.x,  hsize.y,  hsize.z);
							case 2: return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector( hsize.x,  hsize.y,  hsize.z);
							case 3: return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector( hsize.x, -hsize.y,  hsize.z);

							case 4: return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(-hsize.x, -hsize.y, -hsize.z);
							case 5: return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector(-hsize.x,  hsize.y, -hsize.z);
							case 6: return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector( hsize.x,  hsize.y, -hsize.z);
							default:
							case 7: return orientation.getPosition()+AMatrixTranspose(orientation.getMatrix())*AVector( hsize.x, -hsize.y, -hsize.z);
							}
						}


					AVector getPosition() const {return orientation.getPosition();}
					Orientation getOrientation() const {return orientation;}
					float getWidth() const {return hsize.x*2.0f;}
					float getDepth() const {return hsize.y*2.0f;}
					float getHeight() const {return hsize.z*2.0f;}
					AVector getSize() const {return hsize*2.0f;}
					AVector getHalfSize() const {return hsize;}

					void setPosition(const AVector& s) {orientation.setPosition(s);}
					void setSize(const AVector& s) {hsize=s*0.5f;}
				};

			typedef Box Rectangle;
			}
		} /* namespace Math */
	} /* namespace Engine */



