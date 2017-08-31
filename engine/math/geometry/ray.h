/*
 * ray.h
 *
 *  Created on: 29 lip 2017
 *      Author: crm
 */

#pragma once

#include <limits>

#include "basefigure.h"
#include "../amath.h"

namespace Engine
	{
	namespace Math
		{
		namespace Geometry
			{
			class Ray: public BaseFigure
				{
				protected:
					AVector position;
					AVector direction;

				public:
					Ray(): position(), direction() {}
					Ray(const AVector& position, const AVector& direction): position(position), direction(AVectorNormalize(direction)) {}
					Ray(const Ray&)=default;
					~Ray() {}

					Ray& operator=(const Ray&)=default;

					Ray operator+(const AVector& v) const {return Ray(position+v, direction);}
					Ray operator-(const AVector& v) const {return Ray(position-v, direction);}

					Ray& operator+=(const AVector& v) {position+=v; return *this;}
					Ray& operator-=(const AVector& v) {position-=v; return *this;}

					virtual Type getType() const {return Type::RAY;}
					virtual AVector getMinExtent() const {return AVectorReplicate(-std::numeric_limits<float>::infinity());}
					virtual AVector getMaxExtent() const {return AVectorReplicate( std::numeric_limits<float>::infinity());}
					//virtual Boundary getBoundary() const {return Boundary(position, position);}
					virtual AVector getSupport(const AVector&) const {return position;}

					AVector getPosition() const {return position;}
					AVector getDirection() const {return direction;}
				};
			}
		} /* namespace Math */
	} /* namespace Engine */
