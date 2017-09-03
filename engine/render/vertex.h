/*
 * vertex.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

namespace Engine
	{
	namespace Render
		{
		struct VertexPT
			{
			float x, y, z;
			float tx, ty;
			};

		struct VertexPNT
			{
			float x, y, z;
			float nx, ny, nz;
			float tx, ty;
			};

		typedef VertexPT Vertex;

		} /* namespace Render */
	} /* namespace Engine */


