/*
 * ini.h
 *
 *  Created on: 7 wrz 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <string>

namespace Engine
	{
	namespace IO
		{
		class INI
			{
			protected:
				std::string path;
				std::string group;
				std::map<std::string, std::string> values;

			public:
				INI() {}
				INI(const std::string& path, const std::string& group) {init(path, group);}
				~INI() {values.clear();}

				bool init(const std::string& path, const std::string& group);

				std::string operator[](const std::string& key) const {return get(key);}

				std::string get(const std::string& key) const;
			};

		} /* namespace IO */
	} /* namespace Engine */
