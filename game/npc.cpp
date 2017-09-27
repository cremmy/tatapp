/*
 * npc.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include "npc.h"

#include <tinyxml.h>

#include "engine/debug/log.h"
#include "engine/io/resource.h"

using namespace Game;

const Engine::Math::AVector NPC::MARK_COLOR(1.5f, 0.5f, 0.5f, 1.0f);

NPC::NPC(): orientationTargetPercent(1.0f), orientationTargetTime(0.0f),
	name("<unnamed>"), scriptEnabled(false), scriptPath("<not set>"), visible(true),
	collidable(false), targeted(false)
	{
	//
	}

NPC::~NPC()
	{
	//
	}


bool NPC::init(const std::string& path)
	{
	using namespace Engine::Math;
	using namespace Engine::Math::Geometry;

	clear();

	LOG_INFO("NPC.init: Wczytywanie NPC z pliku \"%s\"", path.c_str());

	TiXmlDocument xml;

	char *data;

	LOG_DEBUG("NPC.init: Wczytywanie danych...");
	data=Engine::IO::Resource::load(path);

	if(!data)
		{
		LOG_ERROR("NPC.init: Nie udalo sie wczytac pliku \"%s\"", path.c_str());
		return false;
		}

	LOG_DEBUG("NPC.init: Parsowanie pliku XML...");
	xml.Parse(data);

	if(xml.Error())
		{
		LOG_ERROR("NPC.init: Blad w pliku \"%s\"", path.c_str());
		LOG_ERROR("NPC.init: Linia %d: %s", xml.ErrorRow(), xml.ErrorDesc());
		delete [] data;
		return false;
		}

	/**** Wczytywanie ****/
	TiXmlNode* nnpc=xml.FirstChild("npc");

	if(!nnpc)
		{
		LOG_ERROR("NPC.init: Nie znaleziono tagu \"npc\" w pliku \"%s\"", path.c_str());
		delete [] data;
		return false;
		}

	TiXmlElement* enpc=nnpc->ToElement();

	if(!enpc->Attribute("name"))
		{
		name=path;
		LOG_WARNING("NPC.init: NPC \"%s\" nie ma nazwy", path.c_str());
		}
	else
		{
		name=enpc->Attribute("name");
		LOG_INFO("NPC.init: NPC \"%s\" z pliku \"%s\"", name.c_str(), path.c_str());
		}

	if(!enpc->Attribute("type"))
		{
		LOG_ERROR("NPC.init: Nie zdefiniowano typu NPC \"%s\"", path.c_str());
		delete [] data;
		return false;
		}

	TiXmlElement* etype=nnpc->FirstChildElement(enpc->Attribute("type"));
	if(!etype)
		{
		LOG_ERROR("NPC.init: Nie znaleziono tagu \"%s\" w pliku \"%s\"", enpc->Attribute("type"), path.c_str());
		delete [] data;
		return false;
		}

	if(!load(etype->GetText()))
		{
		LOG_ERROR("NPC.init: Nie udalo sie wczytac grafiki NPC \"%s\"", path.c_str());
		delete [] data;
		return false;
		}

	TiXmlElement* ecol=nnpc->FirstChildElement("collider");
	if(!ecol)
		{
		LOG_WARNING("NPC.init: Nie zdefiniowano hitboxa NPC \"%s\"", path.c_str());
		}
	else
		{
		double x=0.0;
		double y=0.0;
		double z=0.0;
		double w=0.0;
		double d=0.0;
		double h=0.0;

		ecol->Attribute("x", &x);
		ecol->Attribute("y", &y);
		ecol->Attribute("z", &z);

		ecol->Attribute("w", &w);
		ecol->Attribute("d", &d);
		ecol->Attribute("h", &h);

		LOG_DEBUG("NPC.init: [%s][%.2f %.2f %.2f][%.2f %.2f %.2f]", path.c_str(), x, y, z, w, d, h);

		setCollider(Engine::Math::Geometry::AABB(AVector(x, y, z), AVector(w, d, h)));
		}

	delete [] data;

	LOG_SUCCESS("NPC.init: Wczytano NPC \"%s\"", path.c_str());

	return true;
	}

void NPC::update(float dt)
	{
	if(isMovementFinished())
		{
		return;
		}

	if(orientationTargetTime<0.0001f)
		{
		orientationTargetPercent=1.0f;
		}
	else
		{
		orientationTargetPercent+=dt/orientationTargetTime;

		//LOG_DEBUG("percent: %f", orientationTargetPercent);

		if(orientationTargetPercent>1.0f)
			{
			orientationTargetPercent=1.0f;
			}

		orientation=orientationStart.interpolate(orientationTarget, orientationTargetPercent);
		}
	}


Engine::Math::Geometry::Box NPC::getCollider() const
	{
	//const Engine::Math::Orientation& o=orientation;
	const Engine::Math::AVector OFFSET(collider.getPosition());
	return Engine::Math::Geometry::Box(orientation+OFFSET, collider.getSize());
	}

bool NPC::isMovementFinished() const
	{
	if(orientationTargetPercent>=1.0f)
		return true;
	return false;
	}

void NPC::setMovement(const Engine::Math::Orientation& target, float time)
	{
	if(!isMovementFinished())
		{
		LOG_WARNING("NPC.serMovement: Ruch obiektu \"%s\" sie jeszcze nie zakonczyl", name.c_str());
		orientation=orientationTarget;
		}

	orientationStart=orientation;
	orientationTarget=target;
	orientationTargetTime=time;
	orientationTargetPercent=0.0f;
	}
