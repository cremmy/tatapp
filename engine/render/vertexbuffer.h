/*
 * vertexbuffer.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#include "vertex.h"
#include "../math/orientation.h"

namespace Engine
	{
	namespace Graphics
		{
		class ImagePtr;
		class SpritePtr;
		}

	namespace Render
		{
		class Texture;
		class TexturePtr;

		template <typename T>
		class BaseVertexBuffer
			{
			protected:
				GLuint vboid; // Tablica werteksów

				unsigned size;

				std::vector<T> vertices;

				bool isFinalized() const {return vertices.empty() && size>0u;}

			public:
				BaseVertexBuffer(): vboid(0u), size(0u) {}
				virtual ~BaseVertexBuffer() {}

				bool init(T *vertices, unsigned size); // add(vertex)*size + finalize (bardzo możliwe, że będą tam liczne optymalizacje, bo po co wrzucać do vectora)
				bool finalize(); // Przesłanie werteksów z RAMu do grafiki
				void flush();    // Wyczyszczenie buforów
				void clear();    // Zwolnienie całej pamięci

				virtual void bind() const=0;
				virtual void unbind() const=0;

				//GLuint getVAO() const {return vaoid;}
				GLuint getVBO() const {return vboid;}
				unsigned getSize() const {return vertices.empty()?size:vertices.size();}

				bool add(const T& vertex);
			};

		class VertexBufferPT: public BaseVertexBuffer<VertexPT>
			{
			public:
				VertexBufferPT(): BaseVertexBuffer() {}
				virtual ~VertexBufferPT() {}

				virtual void bind() const;
				virtual void unbind() const;

				bool draw(const Math::Orientation& orientation, const TexturePtr& tptr);
				bool draw(const Math::Orientation& orientation, const TexturePtr& tptr, float x, float y, float w, float h);
				bool draw(const Math::Orientation& orientation, const Graphics::ImagePtr& iptr);
				bool draw(const Math::Orientation& orientation, const Graphics::SpritePtr& sptr);
			};

		class VertexBufferPNT: public BaseVertexBuffer<VertexPNT>
			{
			public:
				VertexBufferPNT(): BaseVertexBuffer() {}
				virtual ~VertexBufferPNT() {}

				virtual void bind() const;
				virtual void unbind() const;

//				bool draw(const Math::Orientation& orientation, const TexturePtr& tptr);
//				bool draw(const Math::Orientation& orientation, const TexturePtr& tptr, float x, float y, float w, float h);
//				bool draw(const Math::Orientation& orientation, const Graphics::ImagePtr& iptr);
//				bool draw(const Math::Orientation& orientation, const Graphics::SpritePtr& sptr);
			};

		typedef VertexBufferPT VertexBuffer;

		} /* namespace Render */
	} /* namespace Engine */
