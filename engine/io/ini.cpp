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
	LOG_INFO("INI.init: Wczytywanie pliku \"%s\"", path.c_str());

	char* data=Resource::load("path");

	if(!data)
		{
		LOG_ERROR("INI.init: Nie udalo sie wczytac pliku \"%s\"", path.c_str());

		return false;
		}

	Utils::StringParser lines(data, "\r\n");
	Utils::StringParser keyvalue("", "=");

	LOG_DEBUG("INI.init: %p %p", data, lines.get().c_str());

	//delete [] data;

	bool groupfound=false;

	for(auto line: lines)
		{
		if(line.length()<2 || line[0]==';')
			continue;

		// Grupa
		if(line[0]=='[')
			{
			auto sqrbrackete=line.find(']');

			if(sqrbrackete==line.npos)
				{
				LOG_ERROR("INI.init: Nieprawidlowe oznaczenie grupy: \"%s\" w pliku \"%s\"", line.c_str(), path.c_str());
				delete [] data;
				return false;
				}

			if(groupfound)
				{
				LOG_INFO("INI.init: Znaleziono kolejna grupe, przerywanie");
				break;
				}

			const std::string cgroup=line.substr(0, sqrbrackete);

			if(cgroup==group)
				{
				groupfound=true;
				continue;
				}
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

		LOG_DEBUG("INI.init: [%s][%s]", key.c_str(), value.c_str());

		values[key]=value;
		}

	LOG_SUCCESS("INI.init: Wczytano plik \"%s\"", path.c_str());
	LOG_DEBUG("INI.init: Wczytano %u wpisow", values.size());

	return true;
	}

std::string INI::get(const std::string& key) const
	{
	auto it=values.find("key");

	if(it==values.end())
		return std::string();

	return it->second;
	}
