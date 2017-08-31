/*
 * basefigure.h
 *
 *  Created on: 2 sie 2017
 *      Author: crm
 */

#pragma once

#include "../amath.h"

#include "boundary.h"

namespace Engine
	{
	namespace Math
		{
		namespace Geometry
			{
			class Boundary;

			enum class Type
				{
				AABB,
				BOX,
				CAPSULE,
				LINE,
				MESH,
				POINT,
				POLYGON,
				RAY,
				BALL,
				TRIANGLE,
				};

			class BaseFigure
				{
				public:
					BaseFigure() {}
					virtual ~BaseFigure() {}

					virtual Type getType() const=0;
					virtual AVector getMinExtent() const=0;
					virtual AVector getMaxExtent() const=0;
					virtual Boundary getBoundary() const {return Boundary(getMinExtent(), getMaxExtent());}
				};
			}
		} /* namespace Math */
	} /* namespace Engine */
