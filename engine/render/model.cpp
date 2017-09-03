/*
 * model.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include "model.h"

#include <cstring>
#include <GL/glu.h>

#include "../consts.h"

#include "../debug/log.h"
#include "../io/resource.h"
#include "../utils/stringparser.h"

using namespace Engine::Render;

Model::Model(): uboid(0u)
	{
	//
	}

Model::~Model()
	{
	//
	}


bool Model::load(const std::string& path)
	{
	GLuint err;

	clear();
	glGetError(); // Wyzeruj bledy OpenGL

	LOG_INFO("Model.load: Wczytywanie \"%s\"", path.c_str());

	auto loadMtl=[this](const std::string& path)->bool
		{
		//
		};

	auto loadObj=[this, loadMtl](const std::string& path)->bool
		{
		char* data=IO::Resource::load(path);

		if(!data)
			{
			LOG_ERROR("Model.loadObj: Nie udalo sie wczytac \"%s\"", path.c_str());
			return false;
			}

		Utils::StringParser pfile(data, "\n\r");
		Utils::StringParser pline("", " \t");
		Utils::StringParser pface("", "/", Utils::StringParser::DelimiterMode::EACH);

		LOG_DEBUG("Model.loadObj: [lines: %u]", pfile.count());

		std::vector<Math::AVector> verts;
		verts.reserve(1024);

		std::vector<Math::AVector> uvs;
		uvs.reserve(1024);

		std::vector<Math::AVector> normals;
		normals.reserve(1024);

		for(unsigned i=0u; i<pfile.count(); ++i)
			{
			if(pfile[i].size()<=1u || pfile[i][0]=='#')
				continue;

			pline=pfile[i];

			// Material
			if(pline[0]=="mtllib")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadObj: Nie podano sciezki do materialu");
					continue;
					}

				std::string mpath;

				auto slpos=path.rfind('/');
				if(slpos==path.npos)
					{
					mpath=pline[1];
					}
				else
					{
					mpath=path.substr(0, slpos+1)+pline[1];
					}

				if(!loadMtl(mpath))
					{
					LOG_WARNING("Model.loadObj: Nie udalo sie wczytac materialu z pliku \"%s\"", mpath.c_str());
					//return false; // Brak tekstur nie bedzie krytycznym problemem
					continue;
					}
				}
			// Vertex
			else if(pline[0]=="v")
				{
				if(pline.count()<4)
					{
					LOG_ERROR("Model.loadObj: Za malo wspolrzednych wierzcholka [\"%s\":%u]", path.c_str(), i);
					return false;
					}

				verts.push_back(Math::AVector(pline.toFloat(1), pline.toFloat(2), pline.toFloat(3)));
				}
			// Texture
			else if(pline[0]=="vt")
				{
				if(pline.count()<3)
					{
					LOG_ERROR("Model.loadObj: Za malo wspolrzednych wierzcholka (tekstura) [\"%s\":%u]", path.c_str(), i);
					return false;
					}

				uvs.push_back(Math::AVector(pline.toFloat(1), pline.toFloat(2)));
				}
			// Normal
			else if(pline[0]=="vn")
				{
				if(pline.count()<4)
					{
					LOG_ERROR("Model.loadObj: Za malo wspolrzednych wierzcholka (normalny) [\"%s\":%u]", path.c_str(), i);
					return false;
					}

				normals.push_back(Math::AVector(pline.toFloat(1), pline.toFloat(2), pline.toFloat(3)));
				}
			// Face
			else if(pline[0]=="f")
				{
				if(pline.count()<4)
					{
					LOG_ERROR("Model.loadObj: Wymagane trzy wierzcholki do face'a [\"%s\":%u]", path.c_str(), i);
					return false;
					}

				pface=pline[1];
				unsigned fav=pface.toInt(0);
				unsigned fat=pface.toInt(1);
				unsigned fan=pface.toInt(2);

				pface=pline[2];
				unsigned fbv=pface.toInt(0);
				unsigned fbt=pface.toInt(1);
				unsigned fbn=pface.toInt(2);

				pface=pline[3];
				unsigned fcv=pface.toInt(0);
				unsigned fct=pface.toInt(1);
				unsigned fcn=pface.toInt(2);

				if(fav>=verts.size() || fat>=uvs.size() || fan>=normals.size() ||
				   fbv>=verts.size() || fbt>=uvs.size() || fbn>=normals.size() ||
				   fcv>=verts.size() || fct>=uvs.size() || fcn>=normals.size())
					{
					LOG_ERROR("Model.loadObj: Nieprawidlowy indeks wierzcholka/tekstury/normala [\"%s\":%u]", path.c_str(), i);
					LOG_DEBUG("Model.loadObj: [verts %u][uvs %u][normals %u]", verts.size(), uvs.size(), normals.size());
					return false;
					}

				vbo.add({verts[fav].x, verts[fav].y, verts[fav].z, uvs[fat].x, uvs[fat].y, normals[fan].x, normals[fan].y, normals[fan].z});
				vbo.add({verts[fbv].x, verts[fbv].y, verts[fbv].z, uvs[fbt].x, uvs[fbt].y, normals[fbn].x, normals[fbn].y, normals[fbn].z});
				vbo.add({verts[fcv].x, verts[fcv].y, verts[fcv].z, uvs[fct].x, uvs[fct].y, normals[fcn].x, normals[fcn].y, normals[fcn].z});
				}
			}
		};


	}

bool Model::loadMaterial(const std::string& path)
	{
	//
	}

void Model::clear()
	{
	vbo.clear();
	diffuse=nullptr;
	normal=nullptr;
	}
