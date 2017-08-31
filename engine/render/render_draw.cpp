/*
 * render_draw.cpp
 *
 *  Created on: 14 cze 2017
 *      Author: crm
 */

#include "render.h"

#include "texture.h"
#include "vertex.h"
#include "vertexbuffer.h"
#include "../consts.h"
#include "../debug/assert.h"
#include "../graphics/imageptr.h"
#include "../graphics/spriteptr.h"

using namespace Engine::Render;
using namespace Engine::Math;

void Render::drawLine(const AVector& a, const AVector& b, const Math::AVector& color)
	{
	statePush();

	Vertex data[]={
		{a.x, a.y, a.z,   0.0f, 0.0f},
		{b.x, b.y, b.z,   1.0f, 1.0f}
		};

	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	setShader(baseShaderPrimitive);

	glUniform4fv(baseShaderPrimitive->getUniform(SHADER_UNIFORM_COLOR), 1, &color.x);

	glDisable(GL_TEXTURE_2D);
	glDrawArrays(GL_LINES, 0, 2);
	glEnable(GL_TEXTURE_2D);

	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	statePop();
	}

void Render::drawPolygon(const std::vector<Math::AVector>& vertices, const Math::AVector& color, const Math::AVector& fill)
	{
	statePush();

	std::vector<Vertex> data;
	data.reserve(vertices.size()+1u);

	AVector center=AVector(0, 0, 0);
	for(auto vertex: vertices)
		{
		center+=vertex/(float)vertices.size();
		}

	data.push_back({center.x, center.y, center.z, 0.0f, 0.0f});
	for(auto vertex: vertices)
		{
		data.push_back({vertex.x, vertex.y, vertex.z, 0.0f, 0.0f});
		}
	data.push_back({vertices[0].x, vertices[0].y, vertices[0].z, 0.0f, 0.0f});

	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*data.size(), data.data(), GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	setShader(baseShaderPrimitive);

	glDisable(GL_TEXTURE_2D);
	glUniform4fv(baseShaderPrimitive->getUniform(SHADER_UNIFORM_COLOR), 1, &color.x);
	//glLineWidth(2.0f);
	glDrawArrays(GL_LINE_STRIP, 1, data.size()-1u);
	//glLineWidth(1.0f);
	glUniform4fv(baseShaderPrimitive->getUniform(SHADER_UNIFORM_COLOR), 1, &fill.x);
	glDrawArrays(GL_TRIANGLE_FAN, 0, data.size());
	glEnable(GL_TEXTURE_2D);

	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	statePop();
	}


void Render::draw(const Math::Orientation& orientation, const VertexBuffer& vbo)
	{
	using namespace Engine::Math;

	State& state=states.back();

	glBindBuffer(GL_ARRAY_BUFFER, vbo.getVBO());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	const AMatrix mmodel=orientation.getMatrix();
	glUniformMatrix4fv(state.shader->getUniform(SHADER_UNIFORM_MODEL_MATRIX), 1, GL_TRUE, &mmodel.row[0].x);

	glDrawArrays(GL_QUADS, 0, vbo.getSize());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	}

void Render::draw(const Math::Orientation& orientation, const VertexBuffer& vbo, unsigned first, unsigned last)
	{
	using namespace Engine::Math;

	assert(first<last);
	assert(last<vbo.getSize());

	State& state=states.back();

	glBindBuffer(GL_ARRAY_BUFFER, vbo.getVBO());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	const AMatrix mmodel=orientation.getMatrix();
	glUniformMatrix4fv(state.shader->getUniform(SHADER_UNIFORM_MODEL_MATRIX), 1, GL_TRUE, &mmodel.row[0].x);

	glDrawArrays(GL_QUADS, first, last-first);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	}


void Render::draw(const Orientation& orientation, const Texture& tex)
	{
	//
	}

void Render::draw(const Orientation& orientation, const Texture& tex, float x, float y, float w, float h)
	{
	using namespace Engine::Math;

	assert(tex.getTextureID());

	State& state=states.back();

	const float iw=tex.getW();
	const float ih=tex.getH();

	Vertex data[]=
		{
		{0, 0, 0,  x   /iw,  y   /ih},
		{w, 0, 0, (x+w)/iw,  y   /ih},
		{w,-h, 0, (x+w)/iw, (y+h)/ih},
		{0,-h, 0,  x   /iw, (y+h)/ih},
		};

	bind(0u, tex.getTextureID());

	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	const AMatrix mmodel=orientation.getMatrix();
	glUniformMatrix4fv(state.shader->getUniform(SHADER_UNIFORM_MODEL_MATRIX), 1, GL_TRUE, &mmodel.row[0].x);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	}

void Render::draw(const Orientation& orientation, const TexturePtr& tptr)
	{
	assert(!!tptr);

	draw(orientation, *tptr, 0, 0, tptr->getW(), tptr->getH());
	}

void Render::draw(const Orientation& orientation, const TexturePtr& tptr, float x, float y, float w, float h)
	{
	assert(!!tptr);

	draw(orientation, *tptr, x, y, w, h);
	}

void Render::draw(const Orientation& orientation, const Graphics::ImagePtr& iptr)
	{
	assert(!!iptr);

	draw(orientation, *iptr->getTexture(), iptr->getX(), iptr->getY(), iptr->getW(), iptr->getH());
	}

void Render::draw(const Orientation& orientation, const Graphics::SpritePtr& sptr)
	{
	assert(!!sptr);

	State& state=states.back();

	if(sptr.getShader())
		{
		setShader(sptr.getShader());
		}

	const Graphics::Sprite::Frame& frame=sptr.getCurrentFrame();

	bind(frame.getImage()->getTexture());

	//draw(orientation/*-frame.getOffset()*/, frame.getImage());

	glBindBuffer(GL_ARRAY_BUFFER, sptr->getVertexBuffer().getVBO());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	const AMatrix mmodel=orientation.getMatrix();
	glUniformMatrix4fv(state.shader->getUniform(SHADER_UNIFORM_MODEL_MATRIX), 1, GL_TRUE, &mmodel.row[0].x);

	glDrawArrays(GL_QUADS, frame.getVBOIndex(), 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	}
