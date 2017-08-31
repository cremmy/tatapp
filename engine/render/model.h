/*
 * model.h
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#pragma once

#include "../math/amath.h"

namespace Engine
	{
	namespace Render
		{
		class Model
			{
			public:
				// Ka - ambient
				// Kd - diffuse
				// Ks - specular
				// Ns - specular exponent (0-1000)
				// Ke - emission (odbijanie swiatla)
				// Ni - optical density (zaginanie swiatla; szklo: 1.5)
				// d  - przezroczystosc
				// illum - algorytm oswietlania
				struct Material
					{
					Math::AVector ambient;
					Math::AVector diffuse;
					Math::AVector specular;
					float specularexp;
					float transparency;
					};

				// todo obiekt -> material + VBO

			public:
				Model();
				virtual ~Model();
			};

		} /* namespace Render */
	} /* namespace Engine */
