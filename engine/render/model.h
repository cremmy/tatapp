/*
 * model.h
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#pragma once

#include <string>

#include "../math/amath.h"
#include "vertexbuffer.h"
#include "textureptr.h"

namespace Engine
	{
	namespace Render
		{
		class Model
			{
			protected:
				struct Material
					{
					float ambient[3]; // Ka
					float diffuse[3]; // Kd
					float specular[3]; // Ks
					float specularexp; // Ns
					float transparency; // d
					};

				GLuint uboid;
				VertexBuffer vbo;
				TexturePtr diffuse; // Tekstura koloru
				TexturePtr normal;  // Normal mapa
				Material material;

				bool loadMaterial(const std::string& path);

			public:
				Model();
				~Model();

				bool load(const std::string& path);
				void clear();
			};

		} /* namespace Render */
	} /* namespace Engine */
