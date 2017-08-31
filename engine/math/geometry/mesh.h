/*
 * mesh.h
 *
 *  Created on: 30 lip 2017
 *      Author: crm
 */

#pragma once

#include "basefigure.h"
#include "../amath.h"

namespace Engine
	{
	namespace Math
		{
		namespace Geometry
			{
			class Mesh: public BaseFigure
				{
				/// todo
				protected:
					AVector position;

				public:
					Mesh(): position() {}
					explicit Mesh(const AVector& position): position(position) {}
					Mesh(const Mesh&)=default;
					~Mesh() {}

					Mesh& operator=(const Mesh&)=default;
					operator AVector() const {return position;}

					Mesh operator+(const AVector& v) const {return Mesh(position+v);}
					Mesh operator-(const AVector& v) const {return Mesh(position-v);}

					Mesh& operator+=(const AVector& v) {position+=v; return *this;}
					Mesh& operator-=(const AVector& v) {position-=v; return *this;}

					virtual Type getType() const {return Type::MESH;}
					virtual AVector getMinExtent() const {return position;}
					virtual AVector getMaxExtent() const {return position;}
					//virtual Boundary getBoundary() const {}
					//virtual AVector getSupport(const AVector& dir) const {return AVector();} // TODO

					AVector getPosition() const {return position;}

					void setPosition(const AVector& s) {position=s;}
				};
			}
		} /* namespace Math */
	} /* namespace Engine */
