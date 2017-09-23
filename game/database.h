#pragma once

#include <map>
#include <string>

namespace Game
	{
	class Database
		{
		private:
			std::string name;

			std::map<std::string, std::string> sdb;
			std::map<std::string, int> vdb;
		public:
			Database(): name("")
				{
				}
			Database(const std::string& name): name(name)
				{
				load(name);
				}
			//~Database() {save(name);}

			bool load(const std::string& name);
			bool save(const std::string& name);
			void clear();

			int getVal(const std::string& name);
			std::string getStr(const std::string& name);
			//inline int getVal(const char *name) {return getVal((const std::string&)name);};
			//inline const char* getStr(const char *name) {return getStr((const std::string&)name).c_str();};

			void setVal(const std::string& name, int val);
			void setStr(const std::string& name, const std::string& str);
			//inline void setVal(const char *name, int val) {setVal((const std::string&)name, val);};
			//inline void setStr(const char *name, const char *str) {setStr((const std::string&)name, (const std::string&)str);};
		};
	}

