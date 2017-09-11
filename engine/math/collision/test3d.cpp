/*
 * test3d.cpp
 *
 *  Created on: 29 lip 2017
 *      Author: crm
 */

#include "test3d.h"

#include "../geometry/aabb.h"
#include "../geometry/ball.h"
#include "../geometry/basefigure.h"
#include "../geometry/box.h"
#include "../geometry/capsule.h"
#include "../geometry/line.h"
#include "../geometry/mesh.h"
#include "../geometry/point.h"
#include "../geometry/polygon.h"
#include "../geometry/ball.h"
#include "../geometry/ray.h"
#include "../geometry/triangle.h"

#include "gjk.h"

#include "../../debug/assert.h"

using namespace Engine::Math::Geometry;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test(const AABB& a, const AABB& b)
	{
	const AVector ap=a.getPosition();
	const AVector ahs=a.getHalfSize();
	const AVector bp=b.getPosition();
	const AVector bhs=b.getHalfSize();

	// lol SAT
	if(ap.x+ahs.x<bp.x-bhs.x ||
	   ap.x-ahs.x>bp.x+bhs.x ||
	   ap.y+ahs.y<bp.y-bhs.y ||
	   ap.y-ahs.y>bp.y+bhs.y ||
	   ap.z+ahs.z<bp.z-bhs.z ||
	   ap.z-ahs.z>bp.z+bhs.z)
		return false;

	return true;
	}

bool Engine::Math::Collision::test(const AABB& a, const Box& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const AABB& a, const Capsule& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const AABB& a, const Line& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const AABB& a, const Mesh& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const AABB& a, const Point& b)
	{
	const AVector ap=a.getPosition();
	const AVector ahs=a.getHalfSize();
	const AVector bp=b.getPosition();

	if(ap.x+ahs.x<bp.x ||
	   ap.x-ahs.x>bp.x ||
	   ap.y+ahs.y<bp.y ||
	   ap.y-ahs.y>bp.y ||
	   ap.z+ahs.z<bp.z ||
	   ap.z-ahs.z>bp.z)
		return false;

	return true;
	}

//bool Engine::Math::Collision::test(const AABB& a, const Polygon& b)
//	{
//	assert(false);
//	return false;
//	}

bool Engine::Math::Collision::test(const AABB& a, const Ray& b)
	{
	/*
	Fast Ray-Box Intersection
	by Andrew Woo
	from "Graphics Gems", Academic Press, 1990
	*/

	const int NUMDIM=3;
	enum
		{
		RIGHT=0,
		LEFT=1,
		MIDDLE=2,
		};

	bool inside=true;
	int quadrant[NUMDIM];

	register int i;

	int whichPlane;
	double maxT[NUMDIM];
	double candidatePlane[NUMDIM];

	AVector coord;

	/* Find candidate planes; this loop can be avoided if
   	rays cast all from the eye(assume perpsective view) */
	for (i=0; i<NUMDIM; i++)
		if(b.getPosition()[i] < a.getMinExtent()[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = a.getMinExtent()[i];
			inside = false;
		}else if (b.getPosition()[i] > a.getMaxExtent()[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = a.getMaxExtent()[i];
			inside = false;
		}else	{
			quadrant[i] = MIDDLE;
		}

	/* Ray origin inside bounding box */
	if(inside)	{
		//coord = origin;
		return true;
	}


	/* Calculate T distances to candidate planes */
	for (i = 0; i < NUMDIM; i++)
		if (quadrant[i] != MIDDLE && b.getDirection()[i] !=0.)
			maxT[i] = (candidatePlane[i]-b.getPosition()[i]) / b.getDirection()[i];
		else
			maxT[i] = -1.;

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < NUMDIM; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) return false;
	for (i = 0; i < NUMDIM; i++)
		if (whichPlane != i) {
			coord[i] = b.getPosition()[i] + maxT[whichPlane] *b.getDirection()[i];
			if (coord[i] < a.getMinExtent()[i] || coord[i] > a.getMaxExtent()[i])
				return false;
		} else {
			coord[i] = candidatePlane[i];
		}
	return true;				/* ray hits box */
	}

bool Engine::Math::Collision::test(const AABB& a, const Ball& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const AABB& a, const Triangle& b)
	{
	assert(false);
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test(const Box& a, const Box& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Box& a, const Capsule& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Box& a, const Line& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Box& a, const Mesh& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Box& a, const Point& b)
	{
	const AVector ap=a.getPosition();
	const AVector ahs=a.getHalfSize()*a.getOrientation().getScale();
	const AVector bp=AMatrixTranspose(a.getOrientation().getMatrix())*(b.getPosition()-ap);

	LOG_DEBUG("[%.2f %.2f %.2f] vs [%.2f %.2f %.2f]", bp.x, bp.y, bp.z, ahs.x, ahs.y, ahs.z);

	if( ahs.x<bp.x ||
	   -ahs.x>bp.x ||
	    ahs.y<bp.y ||
	   -ahs.y>bp.y ||
	    ahs.z<bp.z ||
	   -ahs.z>bp.z)
		return false;

	return true;
	}

//bool Engine::Math::Collision::test(const Box& a, const Polygon& b)
//	{
//	assert(false);
//	return false;
//	}

bool Engine::Math::Collision::test(const Box& a, const Ray& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Box& a, const Ball& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Box& a, const Triangle& b)
	{
	assert(false);
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test(const Capsule& a, const Capsule& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Capsule& a, const Line& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Capsule& a, const Mesh& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Capsule& a, const Point& b)
	{
	assert(false);
	return false;
	}

//bool Engine::Math::Collision::test(const Capsule& a, const Polygon& b)
//	{
//	assert(false);
//	return false;
//	}

bool Engine::Math::Collision::test(const Capsule& a, const Ray& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Capsule& a, const Ball& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Capsule& a, const Triangle& b)
	{
	assert(false);
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test(const Line& a, const Line& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Line& a, const Mesh& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Line& a, const Point& b)
	{
	assert(false);
	return false;
	}

//bool Engine::Math::Collision::test(const Line& a, const Polygon& b)
//	{
//	assert(false);
//	return false;
//	}

bool Engine::Math::Collision::test(const Line& a, const Ray& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Line& a, const Ball& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Line& a, const Triangle& b)
	{
	assert(false);
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test(const Mesh& a, const Mesh& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Mesh& a, const Point& b)
	{
	assert(false);
	return false;
	}

//bool Engine::Math::Collision::test(const Mesh& a, const Polygon& b)
//	{
//	assert(false);
//	return false;
//	}

bool Engine::Math::Collision::test(const Mesh& a, const Ray& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Mesh& a, const Ball& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Mesh& a, const Triangle& b)
	{
	assert(false);
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test(const Point& a, const Point& b)
	{
	assert(false);
	return false;
	}

//bool Engine::Math::Collision::test(const Point& a, const Polygon& b)
//	{
//	assert(false);
//	return false;
//	}

bool Engine::Math::Collision::test(const Point& a, const Ray& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Point& a, const Ball& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Point& a, const Triangle& b)
	{
	assert(false);
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//bool Engine::Math::Collision::test(const Polygon& a, const Polygon& b)
//	{
//	assert(false);
//	return false;
//	}
//
//bool Engine::Math::Collision::test(const Polygon& a, const Ray& b)
//	{
//	assert(false);
//	return false;
//	}
//
//bool Engine::Math::Collision::test(const Polygon& a, const Sphere& b)
//	{
//	assert(false);
//	return false;
//	}
//
//bool Engine::Math::Collision::test(const Polygon& a, const Triangle& b)
//	{
//	assert(false);
//	return false;
//	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test(const Ray& a, const Ray& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Ray& a, const Ball& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Ray& a, const Triangle& b)
	{
	assert(false);
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test(const Ball& a, const Ball& b)
	{
	assert(false);
	return false;
	}

bool Engine::Math::Collision::test(const Ball& a, const Triangle& b)
	{
	assert(false);
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test(const Triangle& a, const Triangle& b)
	{
	assert(false);
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::_check3D()
	{
	//	bool test(const Box& a, const Point& b);
	{
	LOG_DEBUG("Geometry.check2D: bool test(const Box& a, const Point& b);");
	Box a(Orientation(AVector(3, 1, 2), AVector(0, 0, 1), AVector(-1, 2, 0)), AVector(2, 1, 3));

	assert(test(a, AVector(3, 1, 1)));

	assert(test(a, AVector(3, 1, 0))==false);
	assert(test(a, AVector(2.75, 1.25, 4))==false);
	}

	// Koniec
	LOG_SUCCESS("Geometry.check2D: Testy zaliczone");
	return true;
	}
