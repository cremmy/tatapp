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

Model::Model()/*: uboid(0u)*/
	{
	//
	}

Model::~Model()
	{
	//
	}


bool Model::load(const std::string& path)
	{
	clear();

	LOG_INFO("Model.load: Wczytywanie \"%s\"", path.c_str());

	shader=ShaderPtr(RENDER_DEFAULT_SHADER_MODEL);

	if(!shader)
		{
		LOG_ERROR("Model.load: Nie udalo sie wczytac shadera modelu");
		return false;
		}

	auto loadMtl=[this](const std::string& path)->bool
		{
		LOG_INFO("Model.loadMtl: Wczytywanie materialow \"%s\"", path.c_str());

		char* data=IO::Resource::load(path);

		if(!data)
			{
			LOG_ERROR("Model.loadMtl: Nie udalo sie wczytac \"%s\"", path.c_str());
			return false;
			}

		Utils::StringParser pfile(data, "\n\r");
		Utils::StringParser pline("", " \t");

		bool mtlfound=false;

		for(unsigned i=0u; i<pfile.count(); ++i)
			{
			if(pfile[i].size()<=1u || pfile[i][0]=='#')
				continue;

			pline=pfile[i];

			if(!mtlfound)
				{
				if(pline[0]=="newmtl")
					{
					LOG_INFO("Model.loadMtl: Znaleziono material \"%s\"", pline.get().c_str());
					mtlfound=true;
					}
				else
					{
					continue;
					}
				}
			else if(pline[0]=="newmtl")
				{
				LOG_WARNING("Model.loadMtl: Znaleziono kolejny material \"%s\", przerywanie wczytywania", pline.get().c_str());
				break;
				}
			else if(pline[0]=="Ka")
				{
				if(pline.count()<4)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				material.ambient[0]=pline.toFloat(1);
				material.ambient[1]=pline.toFloat(2);
				material.ambient[2]=pline.toFloat(3);
				}
			else if(pline[0]=="Kd")
				{
				if(pline.count()<4)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				material.diffuse[0]=pline.toFloat(1);
				material.diffuse[1]=pline.toFloat(2);
				material.diffuse[2]=pline.toFloat(3);
				}
			else if(pline[0]=="Ks")
				{
				if(pline.count()<4)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				material.specular[0]=pline.toFloat(1);
				material.specular[1]=pline.toFloat(2);
				material.specular[2]=pline.toFloat(3);
				}
			else if(pline[0]=="Ns")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				material.specularexp=pline.toFloat(1);
				}
			else if(pline[0]=="d")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				material.transparency=pline.toFloat(1);
				}
			else if(pline[0]=="map_Kd")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				std::string tpath;

				auto slpos=path.rfind('/');
				if(slpos==path.npos)
					{
					tpath=pline[1];
					}
				else
					{
					tpath=path.substr(0, slpos+1)+pline[1];
					}

				material.texDiffuse=TexturePtr(tpath);
				}
			else if(pline[0]=="map_bump")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}


				std::string tpath;

				auto slpos=path.rfind('/');
				if(slpos==path.npos)
					{
					tpath=pline[1];
					}
				else
					{
					tpath=path.substr(0, slpos+1)+pline[1];
					}

				material.texNormal=TexturePtr(tpath);
				}
			}

		LOG_DEBUG("[ambient %.2f %.2f %.2f][diffuse %.2f %.2f %.2f][specular %.2f %.2f %.2f][specular exp %f][transparency %f]",
				material.ambient[0], material.ambient[1], material.ambient[2],
				material.diffuse[0], material.diffuse[1], material.diffuse[2],
				material.specular[0], material.specular[1], material.specular[2],
				material.specularexp,
				material.transparency
				);

		return true;
		};

	auto loadObj=[this, loadMtl](const std::string& path)->bool
		{
		LOG_INFO("Model.loadObj: Wczytywanie modelu \"%s\"", path.c_str());

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

		bool onceft=false;
		bool oncefn=false;

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
					LOG_ERROR("Model.loadObj: \"%s\"", pline.get().c_str());
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
					LOG_ERROR("Model.loadObj: \"%s\"", pline.get().c_str());
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
					LOG_ERROR("Model.loadObj: \"%s\"", pline.get().c_str());
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
					LOG_ERROR("Model.loadObj: \"%s\"", pline.get().c_str());
					return false;
					}

				pface=pline[1];
				unsigned fav=pface.toInt(0)-1;
				unsigned fat=pface.toInt(1)-1;
				unsigned fan=pface.toInt(2)-1;

				pface=pline[2];
				unsigned fbv=pface.toInt(0)-1;
				unsigned fbt=pface.toInt(1)-1;
				unsigned fbn=pface.toInt(2)-1;

				pface=pline[3];
				unsigned fcv=pface.toInt(0)-1;
				unsigned fct=pface.toInt(1)-1;
				unsigned fcn=pface.toInt(2)-1;

				Vertex a;
				Vertex b;
				Vertex c;

				if(fav>=verts.size() || fbv>=verts.size() || fcv>=verts.size())
					{
					LOG_ERROR("Model.loadObj: Bledny lub niezdefiniowany indeks wierzcholka [\"%s\":%u]", path.c_str(), i);
					LOG_ERROR("Model.loadObj: \"%s\"", pline.get().c_str());
					LOG_DEBUG("Model.loadObj: [verts %u][uvs %u][normals %u]", verts.size(), uvs.size(), normals.size());
					return false;
					}

				a={verts[fav].x, verts[fav].y, verts[fav].z, 0, 0, 0, 0, 0};
				b={verts[fbv].x, verts[fbv].y, verts[fbv].z, 0, 0, 0, 0, 0};
				c={verts[fcv].x, verts[fcv].y, verts[fcv].z, 0, 0, 0, 0, 0};

				if(fat>=uvs.size() || fbt>=uvs.size() || fct>=uvs.size())
					{
					if(!onceft)
						{
						onceft=true;
						LOG_WARNING("Model.loadObj: Bledne lub niezdefiniowane wspolrzedne tekstury [\"%s\":%u]", path.c_str(), i);
						LOG_WARNING("Model.loadObj: \"%s\"", pline.get().c_str());
						LOG_DEBUG("Model.loadObj: [verts %u][uvs %u][normals %u]", verts.size(), uvs.size(), normals.size());
						}
					}
				else
					{
					a.tx=uvs[fat].x;
					a.ty=uvs[fat].y;
					b.tx=uvs[fbt].x;
					b.ty=uvs[fbt].y;
					c.tx=uvs[fct].x;
					c.ty=uvs[fct].y;
					}

				if(fan>=normals.size() || fbn>=normals.size() || fcn>=normals.size())
					{
					if(!oncefn)
						{
						oncefn=true;
						LOG_WARNING("Model.loadObj: Bledny lub niezdefiniowany indeks wektora normalnego [\"%s\":%u]", path.c_str(), i);
						LOG_WARNING("Model.loadObj: \"%s\"", pline.get().c_str());
						LOG_DEBUG("Model.loadObj: [verts %u][uvs %u][normals %u]", verts.size(), uvs.size(), normals.size());
						}
					}
				else
					{
					a.nx=normals[fan].x;
					a.ny=normals[fan].y;
					a.nz=normals[fan].z;
					b.nx=normals[fbn].x;
					b.ny=normals[fbn].y;
					b.nz=normals[fbn].z;
					c.nx=normals[fcn].x;
					c.ny=normals[fcn].y;
					c.nz=normals[fcn].z;
					}

				vbo.add(a);
				vbo.add(b);
				vbo.add(c);
				}
			}


		LOG_DEBUG("Model.loadObj: Zwalnianie pamieci");
		delete [] data;

		return true;
		};

	if(!loadObj(path))
		{
		LOG_ERROR("Model.load: Nie udalo sie wczytac modelu \"%s\"", path.c_str());
		return false;
		}

	if(!vbo.finalize())
		{
		return false;
		}

	LOG_INFO("Model.load: Inicjalizacja bufora materialu...");

	//GLenum err;

	glGenBuffers(1, &uboid);

	if(!uboid)
		{
		LOG_ERROR("Model.load: Nie udalo sie zainicjowac UBO [GLid: %u]", uboid);
		LOG_ERROR("Model.load: Blad: %s", gluErrorString(glGetError()));
		return false;
		}

	glBindBuffer(GL_UNIFORM_BUFFER, uboid);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), &material, GL_DYNAMIC_DRAW);

	if(material.texDiffuse)
		shader.uniform("u_texture", material.texDiffuse);
	if(material.texNormal)
		shader.uniform("u_normal", material.texNormal);

	LOG_SUCCESS("Model.load: Wczytano model \"%s\"", path.c_str());

	return true;
	}

void Model::clear()
	{
	vbo.clear();
	shader=nullptr;
	material.texDiffuse=nullptr;
	material.texNormal=nullptr;
	}
