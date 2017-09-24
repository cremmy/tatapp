/*
 * fade.cpp
 *
 *  Created on: 16 maj 2016
 *      Author: crm
 */

#include "fade.h"

#include "engine/debug/log.h"
#include "engine/render/render.h"

using namespace Game;

bool Fade::init()
	{
	layers.reserve(4);

	return true;
	}

void Fade::update(float dt)
	{
	for(auto lay: layers)
		{
		if(!lay)
			continue;
		if(!lay->image)
			continue;

		//lay->sprite.update(dt);

		if(lay->alpha==lay->target)
			continue;

		if(lay->alpha<lay->target)
			{
			lay->alpha+=dt;

			if(lay->alpha>lay->target)
				lay->alpha=lay->target;
			}
		else
			{
			lay->alpha-=dt;

			if(lay->alpha<lay->target)
				lay->alpha=lay->target;
			}
		}
	}

void Fade::print()
	{
	using namespace Engine;
	using namespace Engine::Math;

	//Engine::Render::pushState();
	//Engine::Render::setMode(Engine::Render::MODE_GUI);

	bool once=false;

	//for(auto lay: layers)
	for(std::vector<Layer*>::reverse_iterator it=layers.rbegin(); it!=layers.rend(); ++it)
		{
		Layer *lay=*it;

		if(!lay)
			continue;
		if(lay->alpha<=0.0f)
			continue;
		if(!lay->image)
			continue;

		if(!once)
			{
			Render::getInstance().statePush();
			Render::getInstance().setRenderMode(Render::RenderMode::GUI);
			once=true;
			}

		Render::getInstance().setColor(AVector(1, 1, 1, lay->alpha));
		Render::getInstance().draw(Orientation::GUI, lay->image);
		}

	if(once)
		{
		//Render::getInstance().setRenderMode(Render::RenderMode::NORMAL);
		Render::getInstance().statePop();
		}

	//Engine::Render::popState();
	}

void Fade::clear()
	{
	for(auto lay: layers)
		delete lay;
	layers.clear();
	}


void Fade::set(unsigned layer, const Engine::Graphics::ImagePtr& image, float targetalpha, float alpha)
	{
	if(layer>10u)
		{
		LOG_ERROR("Fade.set: Zbyt daleka warstwa (%u)", layer);
		return;
		}

	if(layer>=layers.size())
		{
		LOG_INFO("Fade.set: Powiekszanie listy warstw (%u)", layer);

		const unsigned lsize=layers.size();

		layers.resize(layer+1);

		for(unsigned i=lsize; i<layers.size(); ++i)
			layers[i]=nullptr;
		}

	if(!layers[layer])
		{
		LOG_INFO("Fade.set: Alokacja warstwy (%u)", layer);

		layers[layer]=new Layer();
		}

	if(alpha>=0.0f)
		layers[layer]->alpha=alpha;
	layers[layer]->target=targetalpha;
	layers[layer]->image=image;
	}

void Fade::set(unsigned layer, float targetalpha, float alpha)
	{
	if(layer>10u)
		{
		LOG_ERROR("Fade.set: Zbyt daleka warstwa (%u)", layer);
		return;
		}

	if(layer>=layers.size())
		{
		LOG_INFO("Fade.set: Powiekszanie listy warstw (%u)", layer);

		layers.resize(layer+1);
		}

	if(!layers[layer])
		{
		LOG_INFO("Fade.set: Alokacja warstwy (%u)", layer);

		layers[layer]=new Layer();
		}

	if(!layers[layer]->image && cache.size()>0u)
		{
		layers[layer]->image=cache[0];
		}

	if(alpha>=0.0f)
		layers[layer]->alpha=alpha;
	layers[layer]->target=targetalpha;
	}

float Fade::getPct(unsigned layer) const
	{
	if(layer>=layers.size())
		return 0.0f;
	if(!layers[layer])
		return 0.0f;

	return layers[layer]->alpha;
	}
