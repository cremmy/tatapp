/*
 * level.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include "level.h"

#include <tinyxml.h>

#include "npc.h"
#include "npcfactory.h"
#include "engine/debug/log.h"
#include "engine/io/resource.h"
#include "engine/math/geometry/point.h"
#include "engine/math/collision/test.h"

#include "engine/render/render.h"

using namespace Game;


Level::Level()
	{
	//
	}

Level::~Level()
	{
	//
	}


bool Level::init(const std::string& path)
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
	TiXmlNode* nlvl=xml.FirstChild("level");

	if(!nlvl)
		{
		LOG_ERROR("Level.init: Nie znaleziono tagu \"level\" w pliku \"%s\"", path.c_str());
		delete [] data;
		return false;
		}

	/**** Collidery ****/
	TiXmlNode* ncol=nlvl->FirstChild("collider");
	while(ncol)
		{
		TiXmlElement* ecol=ncol->ToElement();

		double x1, y1, z1;
		double x2, y2, z2;

		if(!ecol->Attribute("x1", &x1) ||
		   !ecol->Attribute("y1", &y1) ||
		   !ecol->Attribute("z1", &z1) ||
		   !ecol->Attribute("x2", &x2) ||
		   !ecol->Attribute("y2", &y2) ||
		   !ecol->Attribute("z2", &z2))
			{
			LOG_ERROR("Level.init: Collider nie ma poprawnie zdefiniowanych wymiarow, poziom \"%s\"", path.c_str());
			delete [] data;
			return false;
			}

		const AVector a(x1, y1, z1);
		const AVector b(x2, y2, z2);

		colliders.push_back(Engine::Math::Geometry::AABB((a+b)*0.5, AVectorAbsolute(b-a)));

		ncol=ncol->NextSibling("collider");
		}

	/**** NPC ****/
	TiXmlNode* nnpc=nlvl->FirstChild("npc");
	while(nnpc)
		{
		TiXmlElement* enpc=nnpc->ToElement();

		if(!enpc->Attribute("template"))
			{
			LOG_ERROR("Level.init: NPC nie posiada zdefiniowanej sciezki, poziom \"%s\"", path.c_str());
			delete [] data;
			return false;
			}

		NPC* npc=createNPC(enpc->Attribute("template"));

		if(!npc)
			{
			LOG_ERROR("Level.init: Nie udalo sie utworzyc NPC z pliku \"%s\", poziom \"%s\"", enpc->Attribute("template"), path.c_str());
			delete [] data;
			return false;
			}

		if((std::string)enpc->Attribute("visible")=="1")
			{
			LOG_DEBUG("Level.init: NPC \"%s\" jest widzialny, poziom \"%s\"", npc->getName().c_str(), path.c_str());
			npc->setVisibility(true);
			}
		else
			{
			LOG_DEBUG("Level.init: NPC \"%s\" jest niewidzialny, poziom \"%s\"", npc->getName().c_str(), path.c_str());
			npc->setVisibility(false);
			}

		if(enpc->Attribute("collidable") && (std::string)enpc->Attribute("collidable")!="1")
			{
			LOG_DEBUG("Level.init: NPC \"%s\" ma wyaczone kolizje, poziom \"%s\"", npc->getName().c_str(), path.c_str());
			npc->setCollisionEnabled(false);
			}

		TiXmlElement* eorient=enpc->FirstChildElement("orientation");

		if(!eorient)
			{
			LOG_ERROR("Level.init: NPC \"%s\" nie ma zdefiniowanej orientacji, poziom \"%s\"", npc->getName().c_str(), path.c_str());
			delete [] data;
			return false;
			}
		else
			{
			double x, y, z;
			double rx, ry, rz;
			double ux, uy, uz;

			if(!eorient->Attribute("x", &x) ||
			   !eorient->Attribute("y", &y) ||
			   !eorient->Attribute("z", &z) ||
			   !eorient->Attribute("rx", &rx) ||
			   !eorient->Attribute("ry", &ry) ||
			   !eorient->Attribute("rz", &rz) ||
			   !eorient->Attribute("ux", &ux) ||
			   !eorient->Attribute("uy", &uy) ||
			   !eorient->Attribute("uz", &uz))
				{
				LOG_ERROR("Level.init: NPC \"%s\" nie ma poprawnie zdefiniowanej orientacji, poziom \"%s\"", npc->getName().c_str(), path.c_str());
				delete [] data;
				return false;
				}

			double scale=1.0;

			eorient->Attribute("scale", &scale);

			LOG_DEBUG("Level.init: [%s][p %.2f %.2f %.2f][r %.2f %.2f %.2f][u %.2f %.2f %.2f][s %f]",
				npc->getName().c_str(), x, y, z, rx, ry, rz, ux, uy, uz, scale);

			npc->setOrientation(Orientation(AVector(x, y, z), AVector(rx, ry, rz), AVector(ux, uy, uz), scale));
			}

		TiXmlElement* escript=enpc->FirstChildElement("script");

		if(!escript || !escript->Attribute("path"))
			{
			LOG_WARNING("Level.init: NPC \"%s\" nie ma zdefiniowanego skryptu, poziom \"%s\"", npc->getName().c_str(), path.c_str());
			}
		else
			{
			LOG_DEBUG("Level.init: NPC \"%s\" ma skrypt \"%s\", poziom \"%s\"", npc->getName().c_str(), escript->Attribute("path"), path.c_str());
			npc->setScript(escript->Attribute("path"));

			if(!escript->Attribute("enabled") || (std::string)escript->Attribute("enabled")!="1")
				{
				LOG_DEBUG("Level.init: NPC \"%s\" ma ustawiony, ale wylaczony skrypt, poziom \"%s\"", npc->getName().c_str(), path.c_str());
				npc->setScriptEnabled(false);
				}
			}

		npcs.push_back(npc);

		nnpc=nnpc->NextSibling("npc");
		}

	delete [] data;

	LOG_SUCCESS("Level.init: Wczytano poziom \"%s\"", path.c_str());
	LOG_DEBUG("Level.init: [npcs %u]", npcs.size());

	return true;
	}

void Level::update(float dt)
	{
	for(auto npc: npcs)
		{
		npc->update(dt);
		}
	}

void Level::print(float tinterp)
	{
	using namespace Engine::Math;

	/*for(auto col: colliders)
		{
		const AVector cpos=col.getPosition();
		const AVector csize=col.getHalfSize();
		const AVector cu=AVector(0, csize.y, 0);
		const AVector cr=AVector(csize.x, 0, 0);
		const AVector cf=AVector(0, 0, csize.z);

		Engine::Render::getInstance().drawPolygon(
			{
			cpos-cu-cr-cf,
			cpos-cu+cr-cf,
			cpos-cu+cr+cf,
			cpos-cu-cr+cf,
			}, AVector(1, 0.75, 0, 1), AVector(1, 0.75, 0, 0.7));
		Engine::Render::getInstance().drawPolygon(
			{
			cpos+cu-cr-cf,
			cpos+cu+cr-cf,
			cpos+cu+cr+cf,
			cpos+cu-cr+cf,
			}, AVector(1, 0.75, 0, 1), AVector(1, 0.75, 0, 0.7));
		Engine::Render::getInstance().drawPolygon(
			{
			cpos+cu-cr-cf,
			cpos+cu+cr-cf,
			cpos-cu+cr-cf,
			cpos-cu-cr-cf,
			}, AVector(1, 0.75, 0, 1), AVector(1, 0.75, 0, 0.7));
		Engine::Render::getInstance().drawPolygon(
			{
			cpos+cu-cr+cf,
			cpos+cu+cr+cf,
			cpos-cu+cr+cf,
			cpos-cu-cr+cf,
			}, AVector(1, 0.75, 0, 1), AVector(1, 0.75, 0, 0.7));
		Engine::Render::getInstance().drawPolygon(
			{
			cpos+cu-cr-cf,
			cpos+cu-cr+cf,
			cpos-cu-cr+cf,
			cpos-cu-cr-cf,
			}, AVector(1, 0.75, 0, 1), AVector(1, 0.75, 0, 0.7));
		Engine::Render::getInstance().drawPolygon(
			{
			cpos+cu+cr-cf,
			cpos+cu+cr+cf,
			cpos-cu+cr+cf,
			cpos-cu+cr-cf,
			}, AVector(1, 0.75, 0, 1), AVector(1, 0.75, 0, 0.7));
		}*/

	for(auto npc: npcs)
		{
		if(!npc->isVisible())
			continue;

		npc->print(tinterp);

		/*if(npc->isCollidable())
			{
			auto col=npc->getCollider();

			const AVector cpos=col.getPosition();
			const AVector csize=col.getHalfSize();
			const AVector cu=col.getOrientation().getUp()*csize.y;
			const AVector cr=col.getOrientation().getRight()*csize.x;
			const AVector cf=col.getOrientation().getForward()*csize.z;

			Engine::Render::getInstance().drawPolygon(
				{
				cpos-cu-cr-cf,
				cpos-cu+cr-cf,
				cpos-cu+cr+cf,
				cpos-cu-cr+cf,
				}, AVector(1, 0, 0, 1), AVector(1, 0, 0, 0.7));
			Engine::Render::getInstance().drawPolygon(
				{
				cpos+cu-cr-cf,
				cpos+cu+cr-cf,
				cpos+cu+cr+cf,
				cpos+cu-cr+cf,
				}, AVector(1, 0, 0, 1), AVector(1, 0, 0, 0.7));
			Engine::Render::getInstance().drawPolygon(
				{
				cpos+cu-cr-cf,
				cpos+cu+cr-cf,
				cpos-cu+cr-cf,
				cpos-cu-cr-cf,
				}, AVector(1, 0, 0, 1), AVector(1, 0, 0, 0.7));
			Engine::Render::getInstance().drawPolygon(
				{
				cpos+cu-cr+cf,
				cpos+cu+cr+cf,
				cpos-cu+cr+cf,
				cpos-cu-cr+cf,
				}, AVector(1, 0, 0, 1), AVector(1, 0, 0, 0.7));
			Engine::Render::getInstance().drawPolygon(
				{
				cpos+cu-cr-cf,
				cpos+cu-cr+cf,
				cpos-cu-cr+cf,
				cpos-cu-cr-cf,
				}, AVector(1, 0, 0, 1), AVector(1, 0, 0, 0.7));
			Engine::Render::getInstance().drawPolygon(
				{
				cpos+cu+cr-cf,
				cpos+cu+cr+cf,
				cpos-cu+cr+cf,
				cpos-cu+cr-cf,
				}, AVector(1, 0, 0, 1), AVector(1, 0, 0, 0.7));
			}*/
		}
	}

void Level::clear()
	{
	for(auto npc: npcs)
		{
		npc->clear();
		delete npc;
		}

	npcs.clear();
	colliders.clear();
	}


NPC* Level::findNPCByName(const std::string& name)
	{
	for(auto npc: npcs)
		{
		if(npc->getName()!=name)
			continue;

		return npc;
		}

	return nullptr;
	}

NPC* Level::findNPCByRay(const Engine::Math::Geometry::Ray& ray)
	{
	using namespace Engine::Math;

	const float MAX_RANGE=2.0f;
	const int STEPS=20;

	const float STEP=MAX_RANGE/STEPS;

	for(int i=0; i<STEPS; ++i)
		{
		AVector point=ray.getPosition()+ray.getDirection()*(i*STEP);

		for(auto npc: npcs)
			{
			if(!npc->isScriptEnabled() || !npc->isVisible())
				continue;

			//LOG_DEBUG("test %s", npc->getName().c_str());

			if(Collision::test(npc->getCollider(), Geometry::Point(point)))
				return npc;
			}
		}

	return nullptr;
	}

bool Level::test(const Engine::Math::Geometry::AABB& box)
	{
	using namespace Engine::Math;

	for(auto col: colliders)
		{
		if(Collision::test(col, box))
			return true;
		}

	for(auto npc: npcs)
		{
		if(!npc->isCollidable() || !npc->isVisible())
			continue;

		if(Collision::test(npc->getCollider(), box))
			return true;
		}

	return false;
	}
