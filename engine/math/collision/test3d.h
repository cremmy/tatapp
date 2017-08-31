/*
 * test3d.h
 *
 *  Created on: 30 lip 2017
 *      Author: crm
 */

#pragma once

// Na wypadek includowania samego test3d.h
#include "test.h"

namespace Engine
	{
	namespace Math
		{
		namespace Collision
			{
			bool _check3D();

			bool test(const Geometry::AABB& a, const Geometry::AABB& b);
			bool test(const Geometry::AABB& a, const Geometry::Box& b);
			bool test(const Geometry::AABB& a, const Geometry::Capsule& b);
			bool test(const Geometry::AABB& a, const Geometry::Line& b);
			bool test(const Geometry::AABB& a, const Geometry::Mesh& b);
			bool test(const Geometry::AABB& a, const Geometry::Point& b);
			//bool test(const Geometry::AABB& a, const Geometry::Polygon& b);
			bool test(const Geometry::AABB& a, const Geometry::Ray& b);
			bool test(const Geometry::AABB& a, const Geometry::Ball& b);
			bool test(const Geometry::AABB& a, const Geometry::Triangle& b);

			inline bool test(const Geometry::Box& a, const Geometry::AABB& b) {return test(b, a);}
			bool test(const Geometry::Box& a, const Geometry::Box& b);
			bool test(const Geometry::Box& a, const Geometry::Capsule& b);
			bool test(const Geometry::Box& a, const Geometry::Line& b);
			bool test(const Geometry::Box& a, const Geometry::Mesh& b);
			bool test(const Geometry::Box& a, const Geometry::Point& b);
			//bool test(const Geometry::Box& a, const Geometry::Polygon& b);
			bool test(const Geometry::Box& a, const Geometry::Ray& b);
			bool test(const Geometry::Box& a, const Geometry::Ball& b);
			bool test(const Geometry::Box& a, const Geometry::Triangle& b);

			inline bool test(const Geometry::Capsule& a, const Geometry::AABB& b) {return test(b, a);}
			inline bool test(const Geometry::Capsule& a, const Geometry::Box& b) {return test(b, a);}
			bool test(const Geometry::Capsule& a, const Geometry::Capsule& b);
			bool test(const Geometry::Capsule& a, const Geometry::Line& b);
			bool test(const Geometry::Capsule& a, const Geometry::Mesh& b);
			bool test(const Geometry::Capsule& a, const Geometry::Point& b);
			//bool test(const Geometry::Capsule& a, const Geometry::Polygon& b);
			bool test(const Geometry::Capsule& a, const Geometry::Ray& b);
			bool test(const Geometry::Capsule& a, const Geometry::Ball& b);
			bool test(const Geometry::Capsule& a, const Geometry::Triangle& b);

			inline bool test(const Geometry::Line& a, const Geometry::AABB& b) {return test(b, a);}
			inline bool test(const Geometry::Line& a, const Geometry::Box& b) {return test(b, a);}
			inline bool test(const Geometry::Line& a, const Geometry::Capsule& b) {return test(b, a);}
			bool test(const Geometry::Line& a, const Geometry::Line& b);
			bool test(const Geometry::Line& a, const Geometry::Mesh& b);
			bool test(const Geometry::Line& a, const Geometry::Point& b);
			//bool test(const Geometry::Line& a, const Geometry::Polygon& b);
			bool test(const Geometry::Line& a, const Geometry::Ray& b);
			bool test(const Geometry::Line& a, const Geometry::Ball& b);
			bool test(const Geometry::Line& a, const Geometry::Triangle& b);

			inline bool test(const Geometry::Mesh& a, const Geometry::AABB& b) {return test(b, a);}
			inline bool test(const Geometry::Mesh& a, const Geometry::Box& b) {return test(b, a);}
			inline bool test(const Geometry::Mesh& a, const Geometry::Capsule& b) {return test(b, a);}
			inline bool test(const Geometry::Mesh& a, const Geometry::Line& b) {return test(b, a);}
			bool test(const Geometry::Mesh& a, const Geometry::Mesh& b);
			bool test(const Geometry::Mesh& a, const Geometry::Point& b);
			//bool test(const Geometry::Mesh& a, const Geometry::Polygon& b);
			bool test(const Geometry::Mesh& a, const Geometry::Ray& b);
			bool test(const Geometry::Mesh& a, const Geometry::Ball& b);
			bool test(const Geometry::Mesh& a, const Geometry::Triangle& b);

			inline bool test(const Geometry::Point& a, const Geometry::AABB& b) {return test(b, a);}
			inline bool test(const Geometry::Point& a, const Geometry::Box& b) {return test(b, a);}
			inline bool test(const Geometry::Point& a, const Geometry::Capsule& b) {return test(b, a);}
			inline bool test(const Geometry::Point& a, const Geometry::Line& b) {return test(b, a);}
			inline bool test(const Geometry::Point& a, const Geometry::Mesh& b) {return test(b, a);}
			bool test(const Geometry::Point& a, const Geometry::Point& b);
			//bool test(const Geometry::Point& a, const Geometry::Polygon& b);
			bool test(const Geometry::Point& a, const Geometry::Ray& b);
			bool test(const Geometry::Point& a, const Geometry::Ball& b);
			bool test(const Geometry::Point& a, const Geometry::Triangle& b);

			//inline bool test(const Geometry::Polygon& a, const Geometry::AABB& b) {return test(b, a);}
			//inline bool test(const Geometry::Polygon& a, const Geometry::Box& b) {return test(b, a);}
			//inline bool test(const Geometry::Polygon& a, const Geometry::Capsule& b) {return test(b, a);}
			//inline bool test(const Geometry::Polygon& a, const Geometry::Line& b) {return test(b, a);}
			//inline bool test(const Geometry::Polygon& a, const Geometry::Mesh& b) {return test(b, a);}
			//inline bool test(const Geometry::Polygon& a, const Geometry::Point& b) {return test(b, a);}
			//bool test(const Geometry::Polygon& a, const Geometry::Polygon& b);
			//bool test(const Geometry::Polygon& a, const Geometry::Ray& b);
			//bool test(const Geometry::Polygon& a, const Geometry::Sphere& b);
			//bool test(const Geometry::Polygon& a, const Geometry::Triangle& b);

			inline bool test(const Geometry::Ray& a, const Geometry::AABB& b) {return test(b, a);}
			inline bool test(const Geometry::Ray& a, const Geometry::Box& b) {return test(b, a);}
			inline bool test(const Geometry::Ray& a, const Geometry::Capsule& b) {return test(b, a);}
			inline bool test(const Geometry::Ray& a, const Geometry::Line& b) {return test(b, a);}
			inline bool test(const Geometry::Ray& a, const Geometry::Mesh& b) {return test(b, a);}
			inline bool test(const Geometry::Ray& a, const Geometry::Point& b) {return test(b, a);}
			//inline bool test(const Geometry::Ray& a, const Geometry::Polygon& b) {return test(b, a);}
			bool test(const Geometry::Ray& a, const Geometry::Ray& b);
			bool test(const Geometry::Ray& a, const Geometry::Ball& b);
			bool test(const Geometry::Ray& a, const Geometry::Triangle& b);

			inline bool test(const Geometry::Ball& a, const Geometry::AABB& b) {return test(b, a);}
			inline bool test(const Geometry::Ball& a, const Geometry::Box& b) {return test(b, a);}
			inline bool test(const Geometry::Ball& a, const Geometry::Capsule& b) {return test(b, a);}
			inline bool test(const Geometry::Ball& a, const Geometry::Line& b) {return test(b, a);}
			inline bool test(const Geometry::Ball& a, const Geometry::Mesh& b) {return test(b, a);}
			inline bool test(const Geometry::Ball& a, const Geometry::Point& b) {return test(b, a);}
			//inline bool test(const Geometry::Sphere& a, const Geometry::Polygon& b) {return test(b, a);}
			inline bool test(const Geometry::Ball& a, const Geometry::Ray& b) {return test(b, a);}
			bool test(const Geometry::Ball& a, const Geometry::Ball& b);
			bool test(const Geometry::Ball& a, const Geometry::Triangle& b);

			inline bool test(const Geometry::Triangle& a, const Geometry::AABB& b) {return test(b, a);}
			inline bool test(const Geometry::Triangle& a, const Geometry::Box& b) {return test(b, a);}
			inline bool test(const Geometry::Triangle& a, const Geometry::Capsule& b) {return test(b, a);}
			inline bool test(const Geometry::Triangle& a, const Geometry::Line& b) {return test(b, a);}
			inline bool test(const Geometry::Triangle& a, const Geometry::Mesh& b) {return test(b, a);}
			inline bool test(const Geometry::Triangle& a, const Geometry::Point& b) {return test(b, a);}
			//inline bool test(const Geometry::Triangle& a, const Geometry::Polygon& b) {return test(b, a);}
			inline bool test(const Geometry::Triangle& a, const Geometry::Ray& b) {return test(b, a);}
			inline bool test(const Geometry::Triangle& a, const Geometry::Ball& b) {return test(b, a);}
			bool test(const Geometry::Triangle& a, const Geometry::Triangle& b);
			}
		}
	}



