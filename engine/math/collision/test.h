/*
 * test.h
 *
 *  Created on: 29 lip 2017
 *      Author: crm
 */

#pragma once

#include "../amath.h"

namespace Engine
	{
	namespace Math
		{
		namespace Geometry
			{
			class BaseFigure;
			class AABB;
			class Ball;
			class Box;
			class Capsule;
			class Line;
			class Mesh;
			class Point;
			class Polygon;
			class Ray;
			class Triangle;
			}

		namespace Collision
			{
			struct Intersection
				{
				Intersection(): distance(0.0f), a(), b() {}
				Intersection(float distance, const AVector& a, const AVector& b): distance(distance), a(a), b(b) {}

				operator bool() const {return distance<=0.0f;}
				Intersection swap() const {return Intersection(distance, b, a);}

				float distance; // Ujemne - obiekty nachodzą na siebie
				AVector a;      // Najbliższy punkt na obiekcie A
				AVector b;      // Najbliższy punkt na obiekcie B
				};
			}
		}
	}

#include "test3d.h"
#include "test2d.h"

