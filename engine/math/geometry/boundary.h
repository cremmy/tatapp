/*
 * boundary.h
 *
 *  Created on: 2 sie 2017
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
			class Boundary
				{
				protected:
					AVector min;
					AVector max;

				public:
					Boundary(const AVector& min, const AVector& max): min(min), max(max) {}

					bool test(const Boundary& b)
						{
						const Boundary& a=*this;

						const AVector ami=a.min;
						const AVector amx=a.max;
						const AVector bmi=b.min;
						const AVector bmx=b.max;

						if(amx.x<bmi.x ||
						   ami.x>bmx.x ||
						   amx.y<bmi.y ||
						   ami.y>bmx.y ||
						   amx.z<bmi.z ||
						   ami.z>bmx.z)
							return false;

						return true;
						}

					bool test2D(const Boundary& b)
						{
						const Boundary& a=*this;

						const AVector ami=a.min;
						const AVector amx=a.max;
						const AVector bmi=b.min;
						const AVector bmx=b.max;

						if(amx.x<bmi.x ||
						   ami.x>bmx.x ||
						   amx.y<bmi.y ||
						   ami.y>bmx.y)
							return false;

						return true;
						}
				};
			}
		} /* namespace Math */
	} /* namespace Engine */
