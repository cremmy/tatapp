/*
 * isupportpoint.h
 *
 *  Created on: 25 sie 2017
 *      Author: crm
 */

#pragma once

namespace Engine
	{
	namespace Math
		{
		namespace Geometry
			{
			class IHasSupportPoint
				{
				public:
					virtual ~IHasSupportPoint() {}

					virtual AVector getSupport(const AVector& dir) const=0;
				};

			class IHasIndexedSupportPoint: public IHasSupportPoint
				{
				public:
					virtual ~IHasIndexedSupportPoint() {}

					virtual int getSupportID(const AVector& dir) const=0;
					virtual AVector getSupportByID(int id) const=0;
				};
			}
		}
	}


