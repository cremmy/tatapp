/*
 * polygon.cpp
 *
 *  Created on: 24 sie 2017
 *      Author: crm
 */

#include "polygon.h"

using namespace Engine::Math::Geometry;

bool Polygon::isConvex() const
	{
	if(points.size()<=3u)
		return true;

	int sign=0;

	for(unsigned i=2u; i<points.size(); ++i)
		{
		const AVector& pp2=points[i-2u];
		const AVector& pp1=points[i-1u];
		const AVector& pc=points[i];

		const AVector& d1=pp1-pp2;
		const AVector& d2=pp1-pc;

		const AVector& cross=d1^d2;

		if(sign==0)
			{
			if(cross.z<0)
				sign=-1;
			else
				sign=1;
			}
		else if(sign<0 && cross.z>0)
			{
			return false;
			}
		else if(sign>0 && cross.z<0)
			{
			return false;
			}
		}

	return true;
	}


