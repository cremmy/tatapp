/*
 * orientation.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "orientation.h"

using namespace Engine::Math;

const Orientation Orientation::FLAT_XY=Orientation(AVector(), AVector(1, 0, 0), AVector(0, 1, 0));
const Orientation Orientation::FLAT_XZ=Orientation(AVector(), AVector(1, 0, 0), AVector(0, 0, 1));
const Orientation Orientation::FLAT_YZ=Orientation(AVector(), AVector(0, 1, 0), AVector(0, 0, 1));
const Orientation Orientation::GUI=    Orientation(AVector(), AVector(1, 0, 0), AVector(0, 1, 0));

void Orientation::lookAt(const AVector& target, const AVector& from, const AVector& baseup)
	{
	position=from;
	up=AVectorNormalize(baseup);

	forward=AVectorNormalize(target-from);
	right=AVectorNormalize(forward^up);
	up=AVectorNormalize(right^forward);
	}

void Orientation::lookAt(const AVector& target, float angle, float elevation, float distance, float /*roll*/)
	{
	float sa, ca;	// sin, cos; ang
	float se, ce;	// sin, cos; elev

	sincos(-angle/180.0f*M_PI, sa, ca);
	sincos(-elevation/180.0f*M_PI, se, ce);

	forward=AVectorNormalize(AVector(sa*ce, ca*ce, se));
	right=AVector(ca, -sa, 0.0f);
	up=AVectorNormalize(right^forward);
	right=AVectorNormalize(forward^up);

	position=target-forward*distance;
	}

Orientation Orientation::interpolate(const Orientation& target, float p) const
	{
	return Orientation(AVectorLerp(position, target.position, p), AVectorSlerp(right, target.right, p), AVectorSlerp(up, target.up, p));
	}
