/*
 * test2dcontinuous.h
 *
 *  Created on: 25 sie 2017
 *      Author: crm
 */

#pragma once

#include "test.h"

namespace Engine
	{
	namespace Math
		{
		namespace Collision
			{
			bool test2Dcontinuous(const Geometry::AABB& a, const AVector& aspd, const Geometry::AABB& b, const AVector& bspd);
			}
		}
	}

