/*
 * ini.cpp
 *
 *  Created on: 7 wrz 2017
 *      Author: crm
 */

#include "ini.h"
#include "resource.h"
#include "../debug/log.h"
#include "../utils/stringparser.h"

using namespace Engine::IO;

bool INI::init(const std::string& path, const std::string& group)
	{
	if(path==this->path && group==this->group)
		{
		LOG_INFO("INI.init: Plik \"%s\", grupa \"%s\" juz wczytany", path.c_str(), group.c_str());
		return true;
		}

	LOG_INFO("INI.init: Wczytywanie pliku \"%s\"", path.c_str());

	char* data=Resource::load(path);

	if(!data)
		{
		LOG_ERROR("INI.init: Nie udalo sie wczytac pliku \"%s\"", path.c_str());

		return false;
		}

	values.clear();

	Utils::StringParser lines(data, "\r\n");
	Utils::StringParser keyvalue("", "=");

	LOG_DEBUG("INI.init: Linii: %u", lines.count());

	delete [] data;

	bool groupfound=false;

	//for(auto line: lines)
	for(unsigned i=0u; i<lines.count(); ++i)
		{
		const std::string& line=lines[i];

		//LOG_DEBUG("INI.init: \"%s\"", line.c_str());

		if(line.length()<2 || line[0]==';')
			{
			//LOG_DEBUG("INI.init: Komentarz");
			continue;
			}

		// Grupa
		if(line[0]=='[')
			{
			auto sqrbrackete=line.find(']');

			if(sqrbrackete==line.npos)
				{
				LOG_ERROR("INI.init: Nieprawidlowe oznaczenie grupy: \"%s\" w pliku \"%s\"", line.c_str(), path.c_str());
				//delete [] data;
				return false;
				}

			if(groupfound)
				{
				LOG_INFO("INI.init: Znaleziono kolejna grupe, przerywanie");
				break;
				}

			const std::string cgroup=line.substr(1, sqrbrackete-1);

			if(cgroup==group)
				{
				LOG_DEBUG("INI.init: Znaleziono poszukiwana grupe \"%s\"", group.c_str());

				groupfound=true;
				continue;
				}
			else
				{
				LOG_DEBUG("INI.init: Znaleziono grupe \"%s\", ale chcemy \"%s\", poszukiwania trwaja...", cgroup.c_str(), group.c_str());
				}

			continue;
			}

		keyvalue=line;

		if(keyvalue.count()<2)
			{
			LOG_WARNING("INI.init: Pusty klucz \"%s\" w pliku \"%s\"", line.c_str(), path.c_str());
			continue;
			}

		std::string key=keyvalue[0];
		std::string value=keyvalue[1];

		// Super-wydajne trimowanie
		while(key.length()>0 && key[0]<=' ')
			key=key.substr(1);

		while(key.length()>0 && key[key.length()-1]<=' ')
			key=key.substr(0, key.size()-1);

		//LOG_DEBUG("INI.init: [%s][%s]", key.c_str(), value.c_str());

		values[key]=value;
		}

	this->path=path;
	this->group=group;

	LOG_SUCCESS("INI.init: Wczytano plik \"%s\"", path.c_str());
	LOG_DEBUG("INI.init: Wczytano %u wpisow", values.size());

	return true;
	}

std::string INI::get(const std::string& key) const
	{
	auto it=values.find(key);

	if(it==values.end())
		return std::string();

	return it->second;
	}
