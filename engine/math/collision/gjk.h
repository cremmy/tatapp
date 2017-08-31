/*
 * gjkhelper.h
 *
 *  Created on: 3 sie 2017
 *      Author: crm
 */

#pragma once

#include "../amath.h"
#include "../geometry/isupportpoint.h"

namespace Engine
	{
	namespace Math
		{
		namespace Collision
			{
			namespace GJK
				{
				class Simplex
					{
					public:
						struct Vertex
							{
							Vertex():
								id(-1), u(0.0f) {}
							Vertex(int id, const AVector& pos):
								id(id), pos(pos), u(0.0f) {}
							Vertex(int id, const AVector& posa, const AVector& posb):
								id(id), pos(posa-posb), posa(posa), posb(posb), u(0.0f) {}
							Vertex(const Vertex& v):
								id(v.id), pos(v.pos), posa(v.posa), posb(v.posb), u(v.u) {}

							Vertex& operator=(const Vertex& v)
								{
								id=v.id; pos=v.pos; posa=v.posa; posb=v.posb; u=v.u;
								return *this;
								}

							// Identyfikator wierzcholka
							int id;

							// Wspolrzedna wierzcholka oraz punkty z ktorych powstal
							AVector pos;
							AVector posa;
							AVector posb;

							// Wspolrzedna barycentryczna otrzymanego punktu Q
							float u;
							};

					protected:
						unsigned size;
						Vertex verts[3];

					public:
						Simplex(): size(0u) {}

						unsigned getSize() const {return size;}

						bool add(const Vertex& sv);
						bool remove(int idx);

						const Vertex& get(int idx) const;

						AVector solve(const AVector& p);
						void getClosest(AVector& qa, AVector& qb);
					};

				AVector closest(const AVector& p, const AVector& a);
				AVector closest(const AVector& p, const AVector& a, const AVector& b);
				AVector closest(const AVector& p, const AVector& a, const AVector& b, const AVector& c);

				bool test2D(const Geometry::IHasIndexedSupportPoint& a, const Geometry::IHasIndexedSupportPoint& b, const AVector& origin=AVector(0, 0));
				}
			}
		}
	}


