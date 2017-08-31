/*
 * polygon.h
 *
 *  Created on: 30 lip 2017
 *      Author: crm
 */

#pragma once

#include <vector>
#include <initializer_list>

#include "basefigure.h"
#include "isupportpoint.h"
#include "../amath.h"
#include "../../debug/assert.h"

namespace Engine
	{
	namespace Math
		{
		namespace Geometry
			{
			class Polygon: public BaseFigure, public IHasIndexedSupportPoint
				{
				protected:
					std::vector<AVector> points;

				public:
					Polygon(): points() {}
					Polygon(const std::initializer_list<AVector>& list): points(list) {}
					Polygon(const Polygon& poly): points(poly.points) {}
					~Polygon() {}

					Polygon& operator=(const Polygon& poly) {points=poly.points; return *this;}

					bool isConvex() const;

					virtual Type getType() const {return Type::POLYGON;}
					virtual AVector getMinExtent() const
						{
						AVector vmin=points[0];

						for(auto& v: points)
							vmin=AVectorMin(vmin, v);

						return vmin;
						}
					virtual AVector getMaxExtent() const
						{
						AVector vmax=points[0];

						for(auto& v: points)
							vmax=AVectorMax(vmax, v);

						return vmax;
						}
					virtual Boundary getBoundary() const
						{
						AVector vmin=points[0];
						AVector vmax=points[0];

						for(auto& v: points)
							{
							vmin=AVectorMin(vmin, v);
							vmax=AVectorMax(vmax, v);
							}

						return Boundary(vmin, vmax);
						}

					virtual AVector getSupport(const AVector& dir) const
						{
						AVector vsp=points[0];
						float dmx=vsp|dir;

						for(auto& v: points)
							{
							const float d=v|dir;

							if(d<dmx)
								continue;

							dmx=d;
							vsp=v;
							}

						return vsp;
						}

					virtual int getSupportID(const AVector& dir) const
						{
						int vspid=0;
						float dmx=points[0]|dir;

						for(unsigned i=0u; i<points.size(); ++i)
							{
							const AVector& v=points[i];
							const float d=v|dir;

							if(d<dmx)
								continue;

							dmx=d;
							vspid=i;
							}

						return vspid;
						}

					virtual AVector getSupportByID(int id) const
						{
						return points[id];
						}


					void addPoint(const AVector& point)
						{
						points.push_back(point);
						}

					unsigned getPointCount() const {return points.size();}
					AVector getPoint(unsigned i) const {return points[i];}
					const std::vector<AVector>& getPoints() const {return points;}
				};
			}
		} /* namespace Math */
	} /* namespace Engine */
