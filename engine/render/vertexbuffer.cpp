/*
 * vertexbuffer.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "vertexbuffer.h"

#include "textureptr.h"
#include "../debug/log.h"
#include "../graphics/spriteptr.h"


using namespace Engine::Render;

bool VertexBuffer::init(Vertex *vertices, unsigned size)
	{
	GLenum err;
	glGetError(); // Wyzeruj bledy OpenGL

	LOG_INFO("VertexBuffer.init: Inicjalizacja bufora, %u wierzcholkow", size);

	if(!vboid)
		{
		LOG_DEBUG("VertexBuffer.init: Tworzenie VBO");

		glGenBuffers(1, &vboid);

		err=glGetError();
		if(err!=GL_NO_ERROR)
			{
			LOG_ERROR("VertexBuffer.init: Nie udalo sie utworzyc VBO");
			LOG_ERROR("VertexBuffer.init: %s", gluErrorString(err));
			return false;
			}

		LOG_SUCCESS("VertexBuffer.init: Utworzono VertexBuffer [GLid: %u]", vboid);
		}

	/*glGenVertexArrays(1, &vaoid);
	glBindVertexArray(vaoid);*/

	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	LOG_DEBUG("VertexBuffer.init: Wgrywanie danych do VBO [GLid: %u]", vboid);

	this->size=size;
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(Vertex), vertices, GL_STATIC_DRAW);

	err=glGetError();
	if(err!=GL_NO_ERROR)
		{
		LOG_ERROR("VertexBuffer.init: Nie udalo sie wgrac danych do VBO [GLid: %u]", vboid);
		LOG_ERROR("VertexBuffer.init: %s", gluErrorString(err));
		return false;
		}

	LOG_SUCCESS("VertexBuffer.init: Wgranych wierzcholkow: %u [GLid: %u]", size, vboid);

	return true;
	}

bool VertexBuffer::finalize()
	{
	if(isFinalized())
		return true;

	GLenum err;
	glGetError(); // Wyzeruj bledy OpenGL

	LOG_INFO("VertexBuffer.finalize: Finalizowanie bufora, %u wierzcholkow", vertices.size());

	if(!vboid)
		{
		LOG_DEBUG("VertexBuffer.finalize: Tworzenie VBO");

		glGenBuffers(1, &vboid);

		err=glGetError();
		if(err!=GL_NO_ERROR)
			{
			LOG_ERROR("VertexBuffer.finalize: Nie udalo sie utworzyc VBO");
			LOG_ERROR("VertexBuffer.finalize: %s", gluErrorString(err));
			return false;
			}

		LOG_SUCCESS("VertexBuffer.finalize: Utworzono VertexBuffer [GLid: %u]", vboid);
		}

	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	LOG_DEBUG("VertexBuffer.finalize: Wgrywanie danych do VBO [GLid: %u]", vboid);

	this->size=vertices.size();
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

	err=glGetError();
	if(err!=GL_NO_ERROR)
		{
		LOG_ERROR("VertexBuffer.finalize: Nie udalo sie wgrac danych do VBO [GLid: %u]", vboid);
		LOG_ERROR("VertexBuffer.finalize: %s", gluErrorString(err));
		return false;
		}

	vertices.clear();

	LOG_SUCCESS("VertexBuffer.finalize: Wgranych wierzcholkow: %u [GLid: %u]", size, vboid);

	return true;
	}

void VertexBuffer::flush()
	{
	size=0u;
	}

void VertexBuffer::clear()
	{
	LOG_INFO("VertexBuffer.finalize: Czyszczenie bufora [GLid: %u][size: %u]", vboid, vertices.size());

	if(vboid)
		{
		glDeleteBuffers(1, &vboid);
		}

	vboid=0u;

	vertices.clear();
	}


bool VertexBuffer::add(const Vertex& vertex)
	{
	vertices.push_back(vertex);

	return true;
	}

bool VertexBuffer::draw(const Math::Orientation& orientation, const TexturePtr& tptr)
	{
	using namespace Engine::Math;

	const AVector p=orientation.getPosition();
	const AVector r=orientation.getRight();
	const AVector u=orientation.getUp();
	//const AVector f=orientation.getForward();

	const AVector v1=p;
	const AVector v2=p+r*tptr->getW();
	const AVector v3=p+r*tptr->getW()-u*tptr->getH();
	const AVector v4=p               -u*tptr->getH();

	if(!add({v1.x, v1.y, v1.z, 0, 0}) ||
	   !add({v2.x, v2.y, v2.z, 1, 0}) ||
	   !add({v3.x, v3.y, v3.z, 1, 1}) ||
	   !add({v4.x, v4.y, v4.z, 0, 1}))
		return false;
	return true;
	}

bool VertexBuffer::draw(const Math::Orientation& orientation, const TexturePtr& tptr, float x, float y, float w, float h)
	{
	using namespace Engine::Math;

	const AVector p=orientation.getPosition();
	const AVector r=orientation.getRight();
	const AVector u=orientation.getUp();
	//const AVector f=orientation.getForward();

	const AVector v1=p;
	const AVector v2=p+r*w;
	const AVector v3=p+r*w-u*h;
	const AVector v4=p    -u*h;

	const float iw=tptr->getW();
	const float ih=tptr->getH();

	if(!add({v1.x, v1.y, v1.z,  x   /iw,  y   /ih}) ||
	   !add({v2.x, v2.y, v2.z, (x+w)/iw,  y   /ih}) ||
	   !add({v3.x, v3.y, v3.z, (x+w)/iw, (y+h)/ih}) ||
	   !add({v4.x, v4.y, v4.z,  x   /iw, (y+h)/ih}))
		return false;
	return true;
	}

bool VertexBuffer::draw(const Math::Orientation& orientation, const Graphics::ImagePtr& iptr)
	{
	return draw(orientation, iptr->getTexture(), iptr->getX(), iptr->getY(), iptr->getW(), iptr->getH());
	}

bool VertexBuffer::draw(const Math::Orientation& orientation, const Graphics::SpritePtr& sptr)
	{
	return draw(orientation/*-sptr.getCurrentFrame().getOffset()*/, sptr.getCurrentFrame().getImage());
	}

