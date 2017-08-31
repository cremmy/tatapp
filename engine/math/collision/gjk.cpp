/*
 * gjkhelper.cpp
 *
 *  Created on: 3 sie 2017
 *      Author: crm
 */

#include "gjk.h"

#include "../../debug/assert.h"

using namespace Engine::Math;
using namespace Engine::Math::Collision::GJK;

AVector Engine::Math::Collision::GJK::closest(const AVector& p, const AVector& a)
	{
	return a;
	}

AVector Engine::Math::Collision::GJK::closest(const AVector& p, const AVector& a, const AVector& b)
	{
	const AVector n=AVectorNormalize(b-a);
	const float ablen=AVectorLength(b-a).w;

	const float u=((b-p)|n)/ablen;
	const float v=((p-a)|n)/ablen;

	if(u>0.0f && v<0.0f)
		{
		return a;
		}
	else if(u<0.0f && v>0.0f)
		{
		return b;
		}
	else
		{
		return u*a+v*b;
		}
	}

AVector Engine::Math::Collision::GJK::closest(const AVector& p, const AVector& a, const AVector& b, const AVector& c)
	{
	const float uab=((b-p)|AVectorNormalize(b-a))/AVectorLength(b-a).w;
	const float vab=((p-a)|AVectorNormalize(b-a))/AVectorLength(b-a).w;
	const float ubc=((c-p)|AVectorNormalize(c-b))/AVectorLength(c-b).w;
	const float vbc=((p-b)|AVectorNormalize(c-b))/AVectorLength(c-b).w;
	const float uca=((a-p)|AVectorNormalize(a-c))/AVectorLength(a-c).w;
	const float vca=((p-c)|AVectorNormalize(a-c))/AVectorLength(a-c).w;

	//LOG_DEBUG("[ab u %d v %d][bc u %d v %d][ca u %d v %d][uabc %d][vabc %d][wabc %d]", uab<=0, vab<=0, ubc<=0, vbc<=0, uca<=0, vca<=0, uabc<=0, vabc<=0, wabc<=0);
	//LOG_DEBUG("[%d] [%d %d %d]", (int)arabc, (int)arbpc, (int)arapc, (int)arapb);

	if(vab<=0.0f && uca<=0.0f)
		{
		// Region A
		//LOG_DEBUG("A");
		return a;
		}
	else if(uab<=0.0f && vbc<=0.0f)
		{
		//LOG_DEBUG("B");
		// Region B
		return b;
		}
	else if(vca<=0.0f && ubc<=0.0f)
		{
		// region C
		//LOG_DEBUG("C");
		return c;
		}

	// TODO Nie zadziała w trzech wymiarach
	const float arabc=0.5f*((a-b)^(c-b)).z;
	const float arbpc=0.5f*((c-p)^(b-p)).z;
	const float arapc=0.5f*((a-p)^(c-p)).z;
	const float arapb=0.5f*((b-p)^(a-p)).z;

	const float uabc=arbpc/arabc;
	const float vabc=arapc/arabc;
	const float wabc=arapb/arabc;

	if(uab>0.0f && vab>0.0f && wabc<=0.0f)
		{
		// Region AB
		//LOG_DEBUG("AB");
		return uab*a+vab*b;
		}
	else if(ubc>0.0f && vbc>0.0f && uabc<=0.0f)
		{
		// Region BC
		LOG_DEBUG("BC");
		return ubc*b+vbc*c;
		}
	else if(uca>0.0f && vca>0.0f && vabc<=0.0f)
		{
		// Region CA
		LOG_DEBUG("CA");
		return uca*c+vca*a;
		}
	else
		{
		// Wewnątrz
		LOG_DEBUG("ABC");
		return p;
		}
	}


bool Engine::Math::Collision::GJK::test2D(const Geometry::IHasIndexedSupportPoint& a, const Geometry::IHasIndexedSupportPoint& b, const AVector& origin)
	{
	Simplex spx;

	int said;
	int sbid;

	AVector d(0, 1);

//	for(unsigned sa=0u; sa<4; ++sa)
//		{
//		for(unsigned sb=0u; sb<4; ++sb)
//			{
//			const AVector md=a.getSupportByID(sa)-b.getSupportByID(sb);
//
//			LOG_DEBUG(LOG_STR_VECTOR(md));
//			}
//		}

	said=a.getSupportID( d);
	sbid=b.getSupportID(-d);
	spx.add(Simplex::Vertex(said+(sbid<<16), a.getSupportByID(said), b.getSupportByID(sbid)));

	said=a.getSupportID(-d);
	sbid=b.getSupportID( d);
	spx.add(Simplex::Vertex(said+(sbid<<16), a.getSupportByID(said), b.getSupportByID(sbid)));

	AVector q;

	for(unsigned i=0; i<=1000u; ++i)
		{
		assert(i<1000u);
//		if(i==1000u)
//			{
//			LOG_ERROR("GJK.test2D: Zwis");
//			return false;
//			}

		q=spx.solve(origin);

		// Nachodza
		if(spx.getSize()==3u)
			{
			return true;
			}

		d=origin-q;

		// Bliski styk
		if(AVectorLengthSqr(d).w<0.001)
			{
			return true;
			}

		said=a.getSupportID( d);
		sbid=b.getSupportID(-d);

		Simplex::Vertex sv(said+(sbid<<16), a.getSupportByID(said), b.getSupportByID(sbid));

		// Czy punkt wspierajacy jest w sympleksie?
		for(unsigned i=0u; i<spx.getSize(); ++i)
			{
			if(((sv.pos-spx.get(0).pos)|d)<=0.0f)
				{
				return false;
				}

			if(spx.get(i).id!=sv.id)
				{
				continue;
				}

			return false;
			}

		spx.add(sv);
		}

	return false;
	}

/*****************************************************************************/
/**** Sympleks ***************************************************************/
/*****************************************************************************/
bool Simplex::add(const Vertex& sv)
	{
	if(size>=3u)
		return false;

	verts[size++]=sv;

	return true;
	}

bool Simplex::remove(int idx)
	{
	switch(idx)
		{
		case 0:
			verts[0]=verts[1];
			verts[1]=verts[2];
			--size;
		break;

		case 1:
			verts[1]=verts[2];
			--size;
		break;

		case 2:
			--size;
		break;

		default:
			return false;
		}

	return true;
	}

const Simplex::Vertex& Simplex::get(int idx) const
	{
	assert(idx<3);

	return verts[idx];
	}

AVector Simplex::solve(const AVector& p)
	{
	assert(size>0u);

	// Punkt
	if(size==1u)
		{
		verts[0].u=1.0f;
		return verts[0].pos;
		}
	// Odcinek
	else if(size==2u)
		{
		const AVector a=verts[0].pos;
		const AVector b=verts[1].pos;

		const AVector n=AVectorNormalize(b-a);
		const float ablen=AVectorLength(b-a).w;

		const float u=((b-p)|n)/ablen;
		const float v=((p-a)|n)/ablen;

		verts[0].u=u;
		verts[1].u=v;

		if(u>0.0f && v<0.0f)
			{
			// Region A
			remove(1);
			return a;
			}
		else if(u<0.0f && v>0.0f)
			{
			// Region B
			remove(0);
			return b;
			}
		else
			{
			// Region AB
			return u*a+v*b;
			}
		}
	// Trojkat
	else
		{
		const AVector a=verts[0].pos;
		const AVector b=verts[1].pos;
		const AVector c=verts[2].pos;

		const float uab=((b-p)|AVectorNormalize(b-a))/AVectorLength(b-a).w;
		const float vab=((p-a)|AVectorNormalize(b-a))/AVectorLength(b-a).w;
		const float ubc=((c-p)|AVectorNormalize(c-b))/AVectorLength(c-b).w;
		const float vbc=((p-b)|AVectorNormalize(c-b))/AVectorLength(c-b).w;
		const float uca=((a-p)|AVectorNormalize(a-c))/AVectorLength(a-c).w;
		const float vca=((p-c)|AVectorNormalize(a-c))/AVectorLength(a-c).w;

		if(vab<=0.0f && uca<=0.0f)
			{
			// Region A
			remove(2);
			remove(1);
			verts[0].u=1.0f;
			return a;
			}
		else if(uab<=0.0f && vbc<=0.0f)
			{
			// Region B
			remove(2);
			remove(0);
			verts[0].u=1.0f;
			return b;
			}
		else if(vca<=0.0f && ubc<=0.0f)
			{
			// region C
			remove(1);
			remove(0);
			verts[0].u=1.0f;
			return c;
			}

		const float arabc=0.5f*((a-b)^(c-b)).z;
		const float arbpc=0.5f*((c-p)^(b-p)).z;
		const float arapc=0.5f*((a-p)^(c-p)).z;
		const float arapb=0.5f*((b-p)^(a-p)).z;

		const float uabc=arbpc/arabc;
		const float vabc=arapc/arabc;
		const float wabc=arapb/arabc;

		if(uab>0.0f && vab>0.0f && wabc<=0.0f)
			{
			// Region AB
			remove(2);
			verts[0].u=uab;
			verts[1].u=vab;
			return uab*a+vab*b;
			}
		else if(ubc>0.0f && vbc>0.0f && uabc<=0.0f)
			{
			// Region BC
			remove(0);
			verts[0].u=ubc;
			verts[1].u=vbc;
			return ubc*b+vbc*c;
			}
		else if(uca>0.0f && vca>0.0f && vabc<=0.0f)
			{
			// Region CA
			remove(1);
			verts[0].u=uca;
			verts[1].u=vca;
			return uca*c+vca*a;
			}
		else
			{
			// Wewnątrz
			verts[0].u=uabc;
			verts[1].u=vabc;
			verts[2].u=wabc;
			return p;
			}
		}

	return AVector();
	}

void Simplex::getClosest(AVector& qa, AVector& qb)
	{
	qa=AVector();
	qb=AVector();

	for(unsigned i=0u; i<size; ++i)
		{
		qa+=verts[i].posa*verts[i].u;
		qb+=verts[i].posb*verts[i].u;
		}
	}
