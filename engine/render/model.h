/*
 * model.h
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#pragma once

#include <string>

#include "shaderptr.h"
#include "textureptr.h"
#include "vertexbuffer.h"
#include "../math/amath.h"

namespace Engine
	{
	namespace Render
		{
		class Model
			{
			public:
				struct Material
					{
					float ambient[3]; // Ka
					float diffuse[3]; // Kd
					float specular[3]; // Ks
					float specularexp; // Ns
					float transparency; // d

					TexturePtr texDiffuse; // Tekstura koloru
					TexturePtr texNormal;  // Normal mapa
					};

			protected:
				VertexBuffer vbo;
				ShaderPtr shader;
				GLuint uboid;
				Material material;

			public:
				Model();
				~Model();

				bool load(const std::string& path);
				void clear();

				GLuint getUBO() const {return uboid;}
				const VertexBuffer& getVBO() const {return vbo;}
				const ShaderPtr& getShader() const {return shader;}
				const Material& getMaterial() const {return material;}
			};

		} /* namespace Render */
	} /* namespace Engine */
