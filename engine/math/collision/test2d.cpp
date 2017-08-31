/*
 * test2d.cpp
 *
 *  Created on: 30 lip 2017
 *      Author: crm
 */

#include "test2d.h"

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

const float NEAR_ZERO=1e-6;

using namespace Engine::Math::Geometry;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test2D(const BaseFigure& a, const BaseFigure& b)
	{
	/*static bool(*jt[(int)Type::COUNT][(int)Type::COUNT])(const BaseFigure&, const BaseFigure&);*/

	switch(a.getType())
		{
		case Type::AABB:
			switch(b.getType())
				{
				case Type::AABB: return test2D((AABB&)a, (AABB&)b);
				case Type::BOX: return test2D((AABB&)a, (Box&)b);
				case Type::CAPSULE: return test2D((AABB&)a, (Capsule&)b);
				case Type::LINE: return test2D((AABB&)a, (Line&)b);
				case Type::MESH: return test2D((AABB&)a, (Mesh&)b);
				case Type::POINT: return test2D((AABB&)a, (Point&)b);
				case Type::POLYGON: return test2D((AABB&)a, (Polygon&)b);
				case Type::RAY: return test2D((AABB&)a, (Ray&)b);
				case Type::BALL: return test2D((AABB&)a, (Ball&)b);
				case Type::TRIANGLE: return test2D((AABB&)a, (Triangle&)b);
				default: return false;
				}

		case Type::BOX:
			switch(b.getType())
				{
				case Type::AABB: return test2D((Box&)a, (AABB&)b);
				case Type::BOX: return test2D((Box&)a, (Box&)b);
				case Type::CAPSULE: return test2D((Box&)a, (Capsule&)b);
				case Type::LINE: return test2D((Box&)a, (Line&)b);
				case Type::MESH: return test2D((Box&)a, (Mesh&)b);
				case Type::POINT: return test2D((Box&)a, (Point&)b);
				case Type::POLYGON: return test2D((Box&)a, (Polygon&)b);
				case Type::RAY: return test2D((Box&)a, (Ray&)b);
				case Type::BALL: return test2D((Box&)a, (Ball&)b);
				case Type::TRIANGLE: return test2D((Box&)a, (Triangle&)b);
				default: return false;
				}

		case Type::CAPSULE:
			switch(b.getType())
				{
				case Type::AABB: return test2D((Capsule&)a, (AABB&)b);
				case Type::BOX: return test2D((Capsule&)a, (Box&)b);
				case Type::CAPSULE: return test2D((Capsule&)a, (Capsule&)b);
				case Type::LINE: return test2D((Capsule&)a, (Line&)b);
				case Type::MESH: return test2D((Capsule&)a, (Mesh&)b);
				case Type::POINT: return test2D((Capsule&)a, (Point&)b);
				case Type::POLYGON: return test2D((Capsule&)a, (Polygon&)b);
				case Type::RAY: return test2D((Capsule&)a, (Ray&)b);
				case Type::BALL: return test2D((Capsule&)a, (Ball&)b);
				case Type::TRIANGLE: return test2D((Capsule&)a, (Triangle&)b);
				default: return false;
				}

		case Type::LINE:
			switch(b.getType())
				{
				case Type::AABB: return test2D((Line&)a, (AABB&)b);
				case Type::BOX: return test2D((Line&)a, (Box&)b);
				case Type::CAPSULE: return test2D((Line&)a, (Capsule&)b);
				case Type::LINE: return test2D((Line&)a, (Line&)b);
				case Type::MESH: return test2D((Line&)a, (Mesh&)b);
				case Type::POINT: return test2D((Line&)a, (Point&)b);
				case Type::POLYGON: return test2D((Line&)a, (Polygon&)b);
				case Type::RAY: return test2D((Line&)a, (Ray&)b);
				case Type::BALL: return test2D((Line&)a, (Ball&)b);
				case Type::TRIANGLE: return test2D((Line&)a, (Triangle&)b);
				default: return false;
				}

		case Type::MESH:
			switch(b.getType())
				{
				case Type::AABB: return test2D((Mesh&)a, (AABB&)b);
				case Type::BOX: return test2D((Mesh&)a, (Box&)b);
				case Type::CAPSULE: return test2D((Mesh&)a, (Capsule&)b);
				case Type::LINE: return test2D((Mesh&)a, (Line&)b);
				case Type::MESH: return test2D((Mesh&)a, (Mesh&)b);
				case Type::POINT: return test2D((Mesh&)a, (Point&)b);
				case Type::POLYGON: return test2D((Mesh&)a, (Polygon&)b);
				case Type::RAY: return test2D((Mesh&)a, (Ray&)b);
				case Type::BALL: return test2D((Mesh&)a, (Ball&)b);
				case Type::TRIANGLE: return test2D((Mesh&)a, (Triangle&)b);
				default: return false;
				}

		case Type::POINT:
			switch(b.getType())
				{
				case Type::AABB: return test2D((Point&)a, (AABB&)b);
				case Type::BOX: return test2D((Point&)a, (Box&)b);
				case Type::CAPSULE: return test2D((Point&)a, (Capsule&)b);
				case Type::LINE: return test2D((Point&)a, (Line&)b);
				case Type::MESH: return test2D((Point&)a, (Mesh&)b);
				case Type::POINT: return test2D((Point&)a, (Point&)b);
				case Type::POLYGON: return test2D((Point&)a, (Polygon&)b);
				case Type::RAY: return test2D((Point&)a, (Ray&)b);
				case Type::BALL: return test2D((Point&)a, (Ball&)b);
				case Type::TRIANGLE: return test2D((Point&)a, (Triangle&)b);
				default: return false;
				}

		case Type::POLYGON:
			switch(b.getType())
				{
				case Type::AABB: return test2D((Polygon&)a, (AABB&)b);
				case Type::BOX: return test2D((Polygon&)a, (Box&)b);
				case Type::CAPSULE: return test2D((Polygon&)a, (Capsule&)b);
				case Type::LINE: return test2D((Polygon&)a, (Line&)b);
				case Type::MESH: return test2D((Polygon&)a, (Mesh&)b);
				case Type::POINT: return test2D((Polygon&)a, (Point&)b);
				case Type::POLYGON: return test2D((Polygon&)a, (Polygon&)b);
				case Type::RAY: return test2D((Polygon&)a, (Ray&)b);
				case Type::BALL: return test2D((Polygon&)a, (Ball&)b);
				case Type::TRIANGLE: return test2D((Polygon&)a, (Triangle&)b);
				default: return false;
				}

		case Type::RAY:
			switch(b.getType())
				{
				case Type::AABB: return test2D((Ray&)a, (AABB&)b);
				case Type::BOX: return test2D((Ray&)a, (Box&)b);
				case Type::CAPSULE: return test2D((Ray&)a, (Capsule&)b);
				case Type::LINE: return test2D((Ray&)a, (Line&)b);
				case Type::MESH: return test2D((Ray&)a, (Mesh&)b);
				case Type::POINT: return test2D((Ray&)a, (Point&)b);
				case Type::POLYGON: return test2D((Ray&)a, (Polygon&)b);
				case Type::RAY: return test2D((Ray&)a, (Ray&)b);
				case Type::BALL: return test2D((Ray&)a, (Ball&)b);
				case Type::TRIANGLE: return test2D((Ray&)a, (Triangle&)b);
				default: return false;
				}

		case Type::BALL:
			switch(b.getType())
				{
				case Type::AABB: return test2D((Ball&)a, (AABB&)b);
				case Type::BOX: return test2D((Ball&)a, (Box&)b);
				case Type::CAPSULE: return test2D((Ball&)a, (Capsule&)b);
				case Type::LINE: return test2D((Ball&)a, (Line&)b);
				case Type::MESH: return test2D((Ball&)a, (Mesh&)b);
				case Type::POINT: return test2D((Ball&)a, (Point&)b);
				case Type::POLYGON: return test2D((Ball&)a, (Polygon&)b);
				case Type::RAY: return test2D((Ball&)a, (Ray&)b);
				case Type::BALL: return test2D((Ball&)a, (Ball&)b);
				case Type::TRIANGLE: return test2D((Ball&)a, (Triangle&)b);
				default: return false;
				}

		case Type::TRIANGLE:
			switch(b.getType())
				{
				case Type::AABB: return test2D((Triangle&)a, (AABB&)b);
				case Type::BOX: return test2D((Triangle&)a, (Box&)b);
				case Type::CAPSULE: return test2D((Triangle&)a, (Capsule&)b);
				case Type::LINE: return test2D((Triangle&)a, (Line&)b);
				case Type::MESH: return test2D((Triangle&)a, (Mesh&)b);
				case Type::POINT: return test2D((Triangle&)a, (Point&)b);
				case Type::POLYGON: return test2D((Triangle&)a, (Polygon&)b);
				case Type::RAY: return test2D((Triangle&)a, (Ray&)b);
				case Type::BALL: return test2D((Triangle&)a, (Ball&)b);
				case Type::TRIANGLE: return test2D((Triangle&)a, (Triangle&)b);
				default: return false;
				}

		default: return false;
		}
	}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test2D(const AABB& a, const AABB& b)
	{
	const AVector ap=a.getPosition();
	const AVector ahs=a.getHalfSize();
	const AVector bp=b.getPosition();
	const AVector bhs=b.getHalfSize();

	// lol SAT
	if(ap.x+ahs.x<bp.x-bhs.x ||
	   ap.x-ahs.x>bp.x+bhs.x ||
	   ap.y+ahs.y<bp.y-bhs.y ||
	   ap.y-ahs.y>bp.y+bhs.y)
		return false;

	return true;
	}

bool Engine::Math::Collision::test2D(const AABB& a, const Box& b)
	{
	// Zwyczajny SAT
	AVector dirs[]=
		{
		AVector( 1, 0, 0),
		AVector(-1, 0, 0),
		AVector( 0,-1, 0),
		AVector( 0, 1, 0),
		b.getOrientation().getRight(),
		-b.getOrientation().getRight(),
		b.getOrientation().getForward(),
		-b.getOrientation().getForward()
		};

	const unsigned size=sizeof(dirs)/sizeof(dirs[0]);
	for(unsigned i=0u; i<size; ++i)
		{
		float dot=(b.getSupport(-dirs[i])-a.getSupport(dirs[i]))|dirs[i];

		if(dot>0.0f)
			return false;
		}

	return true;
	}

bool Engine::Math::Collision::test2D(const AABB& a, const Capsule& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const AABB& a, const Line& b)
	{
	assert(false); // TODO
	return false;
	}

//bool Engine::Math::Collision::test2D(const AABB& a, const Mesh& b)
//	{
//	assert(false); // TODO
//	return false;
//	}

bool Engine::Math::Collision::test2D(const AABB& a, const Point& b)
	{
	const AVector ap=a.getPosition();
	const AVector ahs=a.getHalfSize();
	const AVector bp=b.getPosition();

	if(ap.x+ahs.x<bp.x ||
	   ap.x-ahs.x>bp.x ||
	   ap.y+ahs.y<bp.y ||
	   ap.y-ahs.y>bp.y)
		return false;

	return true;
	}

bool Engine::Math::Collision::test2D(const AABB& a, const Polygon& b)
	{
	return GJK::test2D(a, b);
	}

bool Engine::Math::Collision::test2D(const AABB& a, const Ray& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const AABB& a, const Ball& b)
	{
	const AVector q=AVectorClamp(a.getPosition()-a.getHalfSize(), b.getPosition(), a.getPosition()+a.getHalfSize());

	if(AVectorLength(q-b.getPosition()).w<=b.getRadius())
		return true;
	return false;
	}

bool Engine::Math::Collision::test2D(const AABB& a, const Triangle& b)
	{
	return GJK::test2D(a, b);
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test2D(const Box& a, const Box& b)
	{
	return GJK::test2D(a, b);
	}

bool Engine::Math::Collision::test2D(const Box& a, const Capsule& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Box& a, const Line& b)
	{
	return GJK::test2D(a, b);
	}

//bool Engine::Math::Collision::test2D(const Box& a, const Mesh& b)
//	{
//	assert(false); // TODO
//	return false;
//	}

bool Engine::Math::Collision::test2D(const Box& a, const Point& b)
	{
	const AVector ap=a.getPosition();
	const AVector ahs=a.getHalfSize();
	const AVector bp=a.getOrientation().getMatrix()*b.getPosition();

	if(ap.x+ahs.x<bp.x ||
	   ap.x-ahs.x>bp.x ||
	   ap.y+ahs.y<bp.y ||
	   ap.y-ahs.y>bp.y)
		return false;

	return true;
	}

bool Engine::Math::Collision::test2D(const Box& a, const Polygon& b)
	{
	return GJK::test2D(a, b);
	}

bool Engine::Math::Collision::test2D(const Box& a, const Ray& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Box& a, const Ball& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Box& a, const Triangle& b)
	{
	return GJK::test2D(a, b);
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test2D(const Capsule& a, const Capsule& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Capsule& a, const Line& b)
	{
	assert(false); // TODO
	return false;
	}

//bool Engine::Math::Collision::test2D(const Capsule& a, const Mesh& b)
//	{
//	assert(false); // TODO
//	return false;
//	}

bool Engine::Math::Collision::test2D(const Capsule& a, const Point& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Capsule& a, const Polygon& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Capsule& a, const Ray& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Capsule& a, const Ball& b)
	{
	const AVector q=GJK::closest(b.getPosition(), a.getPositionA(), a.getPositionB());

	if(AVectorLength(b.getPosition()-q).w<=a.getRadius()+b.getRadius())
		return true;
	return false;
	}

bool Engine::Math::Collision::test2D(const Capsule& a, const Triangle& b)
	{
	assert(false); // TODO
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test2D(const Line& a, const Line& b)
	{
	// Podobnie jak Ray x Ray
	// p+tr  q+us
	// (p+tr) x s = (q+us) x s
	const AVector adir=a.getPositionB()-a.getPositionA();
	const AVector bdir=b.getPositionB()-b.getPositionA();
	const AVector abdist=b.getPositionA()-a.getPositionA();
	const float crossdiradirb=(adir^bdir).z;

	if(crossdiradirb<NEAR_ZERO && crossdiradirb>-NEAR_ZERO)
		{
		const float abdistcrossdira=(abdist^adir).z;

		if(abdistcrossdira<NEAR_ZERO && abdistcrossdira>-NEAR_ZERO)
			{
			// Odcinki na jednej linii
			const float alen=AVectorLengthSqr(adir).w;

			const float t0=(abdist|adir)/alen;
			//const float t1=((abdist-bdir)|adir)/alen;
			const float t1=t0+(bdir|adir)/alen;

			//if(t0>=0.0f || t1>=0.0f)
			if((t0<t1 && t0<=1.0 && t1>=0.0f) || (t1<t0 && t1<=1.0 && t0>=0.0f))
				return true;
			return false;
			}
		else
			{
			// Rownolegle, nie nachodza
			return false;
			}
		}

	const float t=(abdist^bdir).z/crossdiradirb;
	const float u=(abdist^adir).z/crossdiradirb;

	if(0.0f<=t && t<=1.0f && 0.0f<=u && u<=1.0f)
		{
		// Przecinaja sie w punkcie p+tr=q+us
		return true;
		}

	// Nie rownolegle, nie przecinaja sie
	return false;
	}

//bool Engine::Math::Collision::test2D(const Line& a, const Mesh& b)
//	{
//	assert(false); // TODO
//	return false;
//	}

bool Engine::Math::Collision::test2D(const Line& a, const Point& b)
	{
	/*const AVector vab=a.getPositionB()-a.getPositionA();
	const AVector vac=b.getPosition()-a.getPositionA();
	const float cross=(vab^vac).z;

	if(cross>=NEAR_ZERO || cross<=-NEAR_ZERO)
		return false;

	const float dot=vac|vab;

	if(dot<0.0f || dot>AVectorLengthSqr(vab).w)
		return false;

	return true;*/

	return fabs((a.getPositionB().x-a.getPositionA().x)*(b.getPosition().y-a.getPositionA().y) - (b.getPosition().x-a.getPositionA().x)*(a.getPositionB().y-a.getPositionA().y))<NEAR_ZERO;
	}

bool Engine::Math::Collision::test2D(const Line& a, const Polygon& b)
	{
	for(unsigned i=0u; i<b.getPointCount()-1u; ++i)
		{
		if(test2D(a, Line(b.getPoint(i), b.getPoint(i+1u))))
			return true;
		}

	if(test2D(Point(a.getPositionA()), b) || test2D(Point(a.getPositionB()), b))
		return true;

	return false;
	}

bool Engine::Math::Collision::test2D(const Line& a, const Ray& b)
	{
	// Podobnie jak Ray x Ray
	// p+tr  q+us
	// (p+tr) x s = (q+us) x s
	const AVector adir=a.getPositionB()-a.getPositionA();
	const AVector bdir=b.getDirection();
	const AVector abdist=a.getPositionA()-b.getPosition();
	const float crossdiradirb=(bdir^adir).z;

	//LOG_DEBUG("");
	//LOG_DEBUG("[adir %f %f][bdir %f %f][abdist %f %f][cross %f]", adir.x, adir.y, bdir.x, bdir.y, abdist.x, abdist.y, crossdiradirb);

	if(crossdiradirb<NEAR_ZERO && crossdiradirb>-NEAR_ZERO)
		{
		const float abdistcrossdira=(abdist^bdir).z;

		//LOG_DEBUG("[abdistx %f]", abdistcrossdira);

		if(abdistcrossdira<NEAR_ZERO && abdistcrossdira>-NEAR_ZERO)
			{
			// Odcinki na jednej linii
			//const float alen=AVectorLengthSqr(adir).w;

			const float t0=(abdist|bdir);
			const float t1=t0+(adir|bdir);

			//LOG_DEBUG("[t0 %f][t1 %f]", t0, t1);

			if((t0<t1 && /*t0<=1.0 &&*/ t1>=0.0f) || (t1<t0 && /*t1<=1.0 &&*/ t0>=0.0f))
				return true;
			return false;
			}
		else
			{
			// Rownolegle, nie nachodza
			return false;
			}
		}

	const float t=(abdist^bdir).z/crossdiradirb;
	const float u=(abdist^adir).z/crossdiradirb;

	if(0.0f<=t && t<=1.0f && 0.0f<=u /*&& u<=1.0f*/)
		{
		// Przecinaja sie w punkcie p+tr=q+us
		return true;
		}

	// Nie rownolegle, nie przecinaja sie
	return false;
	}

bool Engine::Math::Collision::test2D(const Line& a, const Ball& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Line& a, const Triangle& b)
	{
	assert(false); // TODO
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//bool Engine::Math::Collision::test2D(const Mesh& a, const Mesh& b)
//	{
//	assert(false); // TODO
//	return false;
//	}

//bool Engine::Math::Collision::test2D(const Mesh& a, const Point& b)
//	{
//	assert(false); // TODO
//	return false;
//	}

//bool Engine::Math::Collision::test2D(const Mesh& a, const Polygon& b)
//	{
//	assert(false); // TODO
//	return false;
//	}

//bool Engine::Math::Collision::test2D(const Mesh& a, const Ray& b)
//	{
//	assert(false); // TODO
//	return false;
//	}

//bool Engine::Math::Collision::test2D(const Mesh& a, const Sphere& b)
//	{
//	assert(false); // TODO
//	return false;
//	}

//bool Engine::Math::Collision::test2D(const Mesh& a, const Triangle& b)
//	{
//	assert(false); // TODO
//	return false;
//	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test2D(const Point& a, const Point& b)
	{
	const AVector ap=a.getPosition();
	const AVector bp=b.getPosition();

	return ap.x==bp.x && ap.y==bp.y;
	}

bool Engine::Math::Collision::test2D(const Point& a, const Polygon& b)
	{
	/*const Ray ar(a, AVector(1, 0));

	unsigned passes=0u;

	for(unsigned i=0u; i<b.getPointCount()-1u; ++i)
		{
		if(test2D(a, b.getPoint(i)))
			return true;

		if(test2D(a, Line(b.getPoint(i), b.getPoint(i+1u))))
			return true;

		if(test2D(ar, Line(b.getPoint(i), b.getPoint(i+1u))))
			++passes;
		}

	return passes%2==1;*/
	// Crossing Number Algorithm
	// http://geomalgorithms.com/a03-_inclusion.html
	int cn=0;

	for(unsigned i=0u; i<b.getPointCount()-1u; ++i)
		{
		const AVector aa=a.getPosition();
		const AVector ba=b.getPoint(i);
		const AVector bb=b.getPoint(i+1u);

		if((ba.y<=aa.y && bb.y>aa.y) || (ba.y>aa.y && bb.y<=aa.y))
			{
			const float vt=(aa.y-ba.y)/(bb.y-ba.y);

			if(aa.x<ba.x+vt*(bb.x-ba.x))
				++cn;
			}
		}

	return cn%2==1;

	// Winding Number Algorithm
	// http://geomalgorithms.com/a03-_inclusion.html
	/*auto isLeft=[](const AVector& p, const AVector& la, const AVector& lb)->int
		{
		return (lb.x-la.x)*(p.y-la.y) - (p.x-la.x)*(lb.y-la.y);
		};

	int wn=0;

	for(unsigned i=0u; i<b.getPointCount(); ++i)
		{
		if(test2D(a, b.getPoint(i)))
			return true;
		}

	for(unsigned i=0u; i<b.getPointCount()-1u; ++i)
		{
		const AVector aa=a.getPosition();
		const AVector ba=b.getPoint(i);
		const AVector bb=b.getPoint(i+1u);

		if(ba.y<=aa.y)
			{
			if(bb.y>aa.y) // Krawedz powyzej punktu
				{
				if(isLeft(aa, ba, bb)>0) // punkt na lewo od krawedzi
					{
					++wn;
					}
				}
			}
		else
			{
			if(bb.y<=aa.y) // Krawedz ponizej punktu
				{
				if(isLeft(aa, ba, bb)<0) // punkt na prawo od krawedzi
					{
					--wn;
					}
				}
			}
		}

	return wn>0;*/
	}

bool Engine::Math::Collision::test2D(const Point& a, const Ray& b)
	{
	const AVector adir=a.getPosition()-b.getPosition();
	const float cross=(adir^(b.getDirection())).z;

	if(cross<=NEAR_ZERO || cross>=NEAR_ZERO)
		return false;

	if((adir|b.getDirection())>0.0f)
		return true;

	return false;
	}

bool Engine::Math::Collision::test2D(const Point& a, const Ball& b)
	{
	return AVectorLength(a.getPosition()-b.getPosition()).w<=b.getRadius();
	}

bool Engine::Math::Collision::test2D(const Point& a, const Triangle& b)
	{
	const AVector ap=a.getPosition();
	const AVector ba=b.getPositionA();
	const AVector bb=b.getPositionB();
	const AVector bc=b.getPositionC();

	const AVector ban=(bb-ba)^AVector(0, 0, 1);
	const AVector bbn=(bc-bb)^AVector(0, 0, 1);
	const AVector bcn=(ba-bc)^AVector(0, 0, 1);

	return ((ap-ba)|ban)<=0.0f && ((ap-bb)|bbn)<=0.0f && ((ap-bc)|bcn)<=0.0f;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test2D(const Polygon& a, const Polygon& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Polygon& a, const Ray& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Polygon& a, const Ball& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Polygon& a, const Triangle& b)
	{
	assert(false); // TODO
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test2D(const Ray& a, const Ray& b)
	{
	// Podobnie jak Line x Line
	// p+tr  q+us
	// (p+tr) x s = (q+us) x s
	const AVector abdist=b.getPosition()-a.getPosition();
	const float crossdiradirb=(a.getDirection()^b.getDirection()).z;

	if(crossdiradirb<NEAR_ZERO && crossdiradirb>-NEAR_ZERO)
		{
		const float abdistcrossdira=(abdist^a.getDirection()).z;

		if(abdistcrossdira<NEAR_ZERO && abdistcrossdira>-NEAR_ZERO)
			{
			// Promienie na jednej linii
			//const float alen=AVectorLengthSqr(a.getDirection()).w; // Kierunki sa znormalizowane

			const float t0=(abdist|a.getDirection());
			const float t1=t0+(b.getDirection()|a.getDirection());

			if(t0>=0.0f || t1>=0.0f)
				return true;
			return false;
			}
		else
			{
			// Rownolegle, nie nachodza
			return false;
			}
		}

	const float t=(abdist^b.getDirection()).z/crossdiradirb;
	const float u=(abdist^a.getDirection()).z/crossdiradirb;

	if(0.0f<=t /*&& t<=1.0f*/ && 0.0f<=u /*&& u<=1.0f*/) // t lub u musi byc mniejsze, jesli zamiast _.getDirection() mialbym wektor do drugiego punktu odcinka
		{
		// Przecinaja sie w punkcie p+tr=q+us
		return true;
		}

	// Nie rownolegle, nie przecinaja sie
	return false;
	}

bool Engine::Math::Collision::test2D(const Ray& a, const Ball& b)
	{
	assert(false); // TODO
	return false;
	}

bool Engine::Math::Collision::test2D(const Ray& a, const Triangle& b)
	{
	assert(false); // TODO
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test2D(const Ball& a, const Ball& b)
	{
	return AVectorLength(a.getPosition()-b.getPosition()).w<=(a.getRadius()+b.getRadius());
	}

bool Engine::Math::Collision::test2D(const Ball& a, const Triangle& b)
	{
	assert(false); // TODO
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::test2D(const Triangle& a, const Triangle& b)
	{
	assert(false); // TODO
	return false;
	}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Engine::Math::Collision::_check2D()
	{
//	bool test2D(const AABB& a, const AABB& b);
	{
	LOG_DEBUG("Geometry.check2D: bool test2D(const AABB& a, const AABB& b);");

	AABB a(AVector(0, 0), AVector(2, 2));

	// Boki, styk
	assert(test2D(a, AABB(AVector( 1.5, 0), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(-1.5, 0), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(0,  1.5), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(0, -1.5), AVector(1, 1))));

	// Boki, nachodzenie
	assert(test2D(a, AABB(AVector( 1.2, 0), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(-1.2, 0), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(0,  1.2), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(0, -1.2), AVector(1, 1))));

	// Rogi, styk
	assert(test2D(a, AABB(AVector( 1.5,  1.5), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(-1.5,  1.5), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(-1.5, -1.5), AVector(1, 1))));
	assert(test2D(a, AABB(AVector( 1.5, -1.5), AVector(1, 1))));

	// Rogi, nachodzenie
	assert(test2D(a, AABB(AVector( 1.3,  1.3), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(-1.3,  1.3), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(-1.3, -1.3), AVector(1, 1))));
	assert(test2D(a, AABB(AVector( 1.3, -1.3), AVector(1, 1))));

	// Wnetrze
	assert(test2D(a, AABB(AVector( 0.1, 0.2), AVector(1, 1))));
	assert(test2D(a, AABB(AVector( 0.5, 0), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(-0.5, 0), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(0,  0.5), AVector(1, 1))));
	assert(test2D(a, AABB(AVector(0, -0.5), AVector(1, 1))));

	// Boki, brak kolizji
	assert(test2D(a, AABB(AVector( 1.6, 0), AVector(1, 1)))==false);
	assert(test2D(a, AABB(AVector(-1.7, 0), AVector(1, 1)))==false);
	assert(test2D(a, AABB(AVector(0,  1.8), AVector(1, 1)))==false);
	assert(test2D(a, AABB(AVector(0, -1.9), AVector(1, 1)))==false);

	// Rogi, brak kolizji
	assert(test2D(a, AABB(AVector( 1.6,  1.5), AVector(1, 1)))==false);
	assert(test2D(a, AABB(AVector(-1.7,  1.5), AVector(1, 1)))==false);
	assert(test2D(a, AABB(AVector(-1.5,  1.8), AVector(1, 1)))==false);
	assert(test2D(a, AABB(AVector( 1.5, -1.9), AVector(1, 1)))==false);
	}

//	bool test2D(const AABB& a, const Box& b);
//	bool test2D(const AABB& a, const Capsule& b);
//	bool test2D(const AABB& a, const Line& b);
//	bool test2D(const AABB& a, const Point& b);
//	bool test2D(const AABB& a, const Polygon& b);
//	bool test2D(const AABB& a, const Ray& b);
//	bool test2D(const AABB& a, const Sphere& b);
//	bool test2D(const AABB& a, const Triangle& b);
//
//	bool test2D(const Box& a, const Box& b);
//	bool test2D(const Box& a, const Capsule& b);
//	bool test2D(const Box& a, const Line& b);
//	bool test2D(const Box& a, const Point& b);
//	bool test2D(const Box& a, const Polygon& b);
//	bool test2D(const Box& a, const Ray& b);
//	bool test2D(const Box& a, const Sphere& b);
//	bool test2D(const Box& a, const Triangle& b);
//
//	bool test2D(const Capsule& a, const Capsule& b);
//	bool test2D(const Capsule& a, const Line& b);
//	bool test2D(const Capsule& a, const Point& b);
//	bool test2D(const Capsule& a, const Polygon& b);
//	bool test2D(const Capsule& a, const Ray& b);
//	bool test2D(const Capsule& a, const Sphere& b);
//	bool test2D(const Capsule& a, const Triangle& b);
//
//	bool test2D(const Line& a, const Line& b);
	{
	LOG_DEBUG("Geometry.check2D: bool test2D(const Line& a, const Line& b);");
	Line a(AVector(-2, -1), AVector(2, 1));

	// Pozioma
	assert(test2D(a, Line(AVector(-2, 0), AVector(2, 0))));
	assert(test2D(a, Line(AVector(2, 0), AVector(-2, 0))));
	// Pionowa
	assert(test2D(a, Line(AVector(-1, -1), AVector(-1, 1))));
	assert(test2D(a, Line(AVector(-1, 1), AVector(-1, -1))));
	// Skosna
	assert(test2D(a, Line(AVector(-2, 0), AVector(-1, -1))));
	assert(test2D(a, Line(AVector(-1, -1), AVector(-2, 0))));
	assert(test2D(a, Line(AVector(1, -1), AVector(-2, 1))));
	assert(test2D(a, Line(AVector(-2, 1), AVector(1, -1))));
	// Nachodzenie na identyczna
	assert(test2D(a, Line(AVector(-2, -1), AVector(2, 1))));
	assert(test2D(a, Line(AVector(2, 1), AVector(-2, -1))));
	// Mniejsza na wiekszej
	assert(test2D(a, Line(AVector(-1, -0.5), AVector(1, 0.5))));
	assert(test2D(a, Line(AVector(1, 0.5), AVector(-1, -0.5))));
	// Nachodzenie, jeden koniec
	assert(test2D(a, Line(AVector(-4, -2), AVector(-1, -0.5))));
	assert(test2D(a, Line(AVector(-1, -0.5), AVector(-4, -2))));
	// Nachodzenie, drugi koniec
	assert(test2D(a, Line(AVector(4, 2), AVector(1, 0.5))));
	assert(test2D(a, Line(AVector(1, 0.5), AVector(4, 2))));

	// Brak kolizji
	assert(test2D(a, Line(AVector(4, 0), AVector(6, -2)))==false);
	assert(test2D(a, Line(AVector(6, -2), AVector(4, 0)))==false);

	assert(test2D(a, Line(AVector(-4, 2), AVector(-2, -4)))==false);
	assert(test2D(a, Line(AVector(-2, -4), AVector(-4, 2)))==false);

	assert(test2D(a, Line(AVector(4, 2), AVector(2, -4)))==false);
	assert(test2D(a, Line(AVector(2, -4), AVector(4, 2)))==false);

	// Brak kolizji, rownolegle
	assert(test2D(a, Line(AVector(-2, -2), AVector(2, 0)))==false);
	assert(test2D(a, Line(AVector(2, 0), AVector(-2, -2)))==false);
	assert(test2D(a, Line(AVector(-2, 0), AVector(2, 2)))==false);
	assert(test2D(a, Line(AVector(2, 2), AVector(-2, 0)))==false);

	// Brak kolizji, wspolliniowe

	assert(test2D(a, Line(AVector(-3, -1.5), AVector(-4, -2)))==false);
	assert(test2D(a, Line(AVector(-4, -2), AVector(-3, -1.5)))==false);
	assert(test2D(a, Line(AVector(3, 1.5), AVector(4, 2)))==false);
	assert(test2D(a, Line(AVector(4, 2), AVector(3, 1.5)))==false);
	}
//	bool test2D(const Line& a, const Point& b);
	{
	LOG_DEBUG("Geometry.check2D: bool test2D(const Line& a, const Point& b);");
	Line a(AVector(-2, 0), AVector(2, 2));

	assert(test2D(a, Point(AVector(-2, 0))));
	assert(test2D(a, Point(AVector(2, 2))));
	assert(test2D(a, Point(AVector(0, 1))));

	assert(test2D(a, Point(AVector(-3, -1)))==false);
	assert(test2D(a, Point(AVector(3, 3)))==false);
	assert(test2D(a, Point(AVector(-1, 1)))==false);
	}

//	bool test2D(const Line& a, const Polygon& b);
//	bool test2D(const Line& a, const Ray& b);
	{
	LOG_DEBUG("Geometry.check2D: bool test2D(const Line& a, const Ray& b);");
	Ray b(AVector(-2, 0), AVector(2, 1));

	assert(test2D(Line(AVector(-2, 2), AVector(-2, -1)), b));
	assert(test2D(Line(AVector(2, 3), AVector(2, -1)), b));
	assert(test2D(Line(AVector(-6, -2), AVector(0, 1)), b));
	assert(test2D(Line(AVector(4, 3), AVector(2, 2)), b));
	assert(test2D(Line(AVector(2, 2), AVector(4, 3)), b));

	assert(test2D(Line(AVector(-3, 2), AVector(-2, -2)), b)==false);
	assert(test2D(Line(AVector(-6, -2), AVector(-3, -0.5)), b)==false);
	}
//	bool test2D(const Line& a, const Sphere& b);
//	bool test2D(const Line& a, const Triangle& b);
//
//	bool test2D(const Point& a, const Point& b);
	{
	LOG_DEBUG("Geometry.check2D: bool test2D(const Point& a, const Point& b);");

	assert(test2D(AVector(0, 0), AVector(0, 0)));
	assert(test2D(AVector(1, 2), AVector(1, 2)));

	assert(test2D(AVector(-1, 0), AVector(0, 0))==false);
	assert(test2D(AVector(0, -1), AVector(0, 0))==false);
	assert(test2D(AVector(1, -1), AVector(0, 0))==false);
	}
//	bool test2D(const Point& a, const Polygon& b);
	{
	LOG_DEBUG("Geometry.check2D: bool test2D(const Point& a, const Polygon& b);");
	Polygon b({AVector(2, 1), AVector(3, 5), AVector(8, 7), AVector(7, 4), AVector(10, 6), AVector(11, 2), AVector(5, 1)});

	// Na wierzcholkach
	assert(test2D(AVector(2, 1), b));
	assert(test2D(AVector(3, 5), b));
	//assert(test2D(AVector(8, 7), b));
	assert(test2D(AVector(7, 4), b));
	//assert(test2D(AVector(10, 6), b));
	//assert(test2D(AVector(11, 2), b));
	//assert(test2D(AVector(5, 1), b));

	// Na krawędziach
	assert(test2D(AVector(2.5, 3), b));
	assert(test2D(AVector(5.5, 6), b));
	//assert(test2D(AVector(7.5, 5.5), b));
	assert(test2D(AVector(8.5, 5), b));
	//assert(test2D(AVector(10.5, 4), b));
	//assert(test2D(AVector(8, 1.5), b));
	assert(test2D(AVector(4, 1), b));

	// Wewnątrz
	assert(test2D(AVector(3, 2), b));
	assert(test2D(AVector(4, 4), b));
	assert(test2D(AVector(6, 6), b));

	// Wewnątrz, koło wierzchołków/krawędzi które nie przechodzą normalnych testów
	assert(test2D(AVector(7.98, 6.98), b));
	assert(test2D(AVector(9.98, 5.98), b));
	assert(test2D(AVector(10.98, 2), b));
	assert(test2D(AVector(5, 1.01), b));
	assert(test2D(AVector(7.48, 5.5), b));
	assert(test2D(AVector(10.48, 4), b));
	assert(test2D(AVector(8, 1.51), b));
	}
//	bool test2D(const Point& a, const Ray& b);
//	bool test2D(const Point& a, const Sphere& b);
//	bool test2D(const Point& a, const Triangle& b);
//
//	bool test2D(const Polygon& a, const Polygon& b);
//	bool test2D(const Polygon& a, const Ray& b);
//	bool test2D(const Polygon& a, const Sphere& b);
//	bool test2D(const Polygon& a, const Triangle& b);
//
//	bool test2D(const Ray& a, const Ray& b);
	{
	LOG_DEBUG("Geometry.check2D: bool test2D(const Ray& a, const Ray& b);");
	Ray a(AVector(-2, -1), AVector(2, 1));

	// Ponizej, w gore
	assert(test2D(a, Ray(AVector(1, 0), AVector(0, 1))));
	// Powyzej, w dol
	assert(test2D(a, Ray(AVector(0, 3), AVector(0, -1))));
	// Na lewo, w prawo
	assert(test2D(a, Ray(AVector(-2, 4), AVector(1, 0))));
	// Na prawo, w lewo
	assert(test2D(a, Ray(AVector(10, 2), AVector(-1, 0))));
	// Po skosie w prawo-dol
	assert(test2D(a, Ray(AVector(4, 4), AVector(1, -0.5))));
	// Po skosie w prawo-gore
	assert(test2D(a, Ray(AVector(-2, -4), AVector(0.3, 1))));
	// Po skosie w lewo-gore
	assert(test2D(a, Ray(AVector(2, -4), AVector(-0.3, 1))));
	// Po skosie w lewo-dol
	assert(test2D(a, Ray(AVector(4, 4), AVector(-1, -1))));

	// Nachodzi
	assert(test2D(a, Ray(AVector(-2, -1), AVector(2, 1))));
	// Nachodzi, przesuniete
	assert(test2D(a, Ray(AVector(2, 1), AVector(2, 1))));
	// Nachodzi, przeciwny kierunek
	assert(test2D(a, Ray(AVector(2, 1), AVector(-2, -1))));
	// Wspolny poczatek
	assert(test2D(a, Ray(AVector(-2, -1), AVector(-2, 1))));

	// Rownolegle, nie przecinaja sie, przesuniete
	assert(test2D(a, Ray(AVector(-2, -2), AVector(2, 1)))==false);
	// Rownolegle, nie przecinaja sie, wspolna linia
	assert(test2D(a, Ray(AVector(-4, -2), AVector(-2, -1)))==false);

	// Brak przeciecia
	assert(test2D(a, Ray(AVector(-1, -3), AVector(-1, 1)))==false);
	assert(test2D(a, Ray(AVector(-3, -1), AVector(1, -1)))==false);
	assert(test2D(a, Ray(AVector(-1, 2), AVector(-1, -1)))==false);
	assert(test2D(a, Ray(AVector(-4, -2), AVector(3, 1)))==false);
	}
//	bool test2D(const Ray& a, const Sphere& b);
//	bool test2D(const Ray& a, const Triangle& b);
//
//	bool test2D(const Sphere& a, const Sphere& b);
//	bool test2D(const Sphere& a, const Triangle& b);
//
//	bool test2D(const Triangle& a, const Triangle& b);

	// Koniec
	LOG_SUCCESS("Geometry.check2D: Testy zaliczone");
	return true;
	}
