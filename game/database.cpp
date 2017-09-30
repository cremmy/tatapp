#include <fstream>
#include <cstdio>

#include "database.h"

#include "engine/debug/log.h"

using namespace Game;

bool Database::load(const std::string& name)
	{
	//printf("OCB?\n");
	if(name.length()<2)
		return 0;
	this->name=name;
	Database::clear();
	unsigned int c=0;	// Ilosc wpisow
	unsigned int len=0;	// Dlugosc klucza
	std::string key;	// Klucz
	std::ifstream in(name.c_str(), std::ios::binary);

	if(!in)
		{
		LOG_ERROR("Database.load: Nie udalo sie otworzyc pliku \"%s\"\n", name.c_str());
		return 0;
		}

// Wczytywanie const std::string&ow
	in.read(reinterpret_cast<char *>(&c), sizeof(int));
//	printf("INFO: Database.load: Ilosc zmiennych tekstowych: %u\n", c);

	if(c>0&&!in.fail())
		{
		for(unsigned int i=0; i<c; i++)
			{
			// Wczytywanie klucza
			in.read(reinterpret_cast<char *>(&len), sizeof(int));
			if(len==0)
				{
				LOG_ERROR("Database.load: Zerowa dlugosc klucza (str)\n");
				return 0;
				}
			try
				{
				char *keyTmp=new char[len+1];
				in.read(keyTmp, len);
				keyTmp[len]='\0';
				key=keyTmp;
				delete[] keyTmp;
				}
			catch(std::bad_alloc &)
				{
				LOG_ERROR("Database.load: Nie udalo sie zajac pamieci na klucz (str)\n");
				return 0;
				}
			// Wczytywanie danych
			in.read(reinterpret_cast<char *>(&len), sizeof(int));
			if(len==0)
				{
				LOG_ERROR("Database.load: Zerowa dlugosc klucza (str) - %u\n", (unsigned int)in.tellg());
				//return 0;
				continue;
				}
			try
				{
				char *val=new char[len+1];
				in.read(val, len);
				val[len]='\0';
				setStr(key, val);
				delete[] val;
				}
			catch(std::bad_alloc &)
				{
				LOG_ERROR("Database.load: Nie udalo sie zajac pamieci na klucz (str)\n");
				return 0;
				}
			}
		}
	// Wczytywanie intow
	in.read(reinterpret_cast<char *>(&c), sizeof(int));
//	printf("INFO: Database.load: Ilosc zmiennych liczbowych: %u\n", c);
	if(c>0&&!in.fail())
		{
		for(unsigned int i=0; i<c; i++)
			{
			// Wczytywanie klucza
			in.read(reinterpret_cast<char *>(&len), sizeof(int));
			if(len==0)
				{
				LOG_ERROR("Database.load: Zerowa dlugosc klucza (val) - %u\n", (unsigned int)in.tellg());
				//return 0;
				continue;
				}
			try
				{
				char *keyTmp=new char[len+1];
				in.read(keyTmp, len);
				keyTmp[len]='\0';
				key=keyTmp;
				delete[] keyTmp;
				}
			catch(std::bad_alloc &)
				{
				LOG_ERROR("Database.load: Nie udalo sie zajac pamieci na klucz (val)\n");
				return 0;
				}
			// Wczytywanie danych
			int val;
			in.read(reinterpret_cast<char *>(&val), sizeof(int));
			setVal(key, val);
			}
		}

	in.close();

	LOG_DEBUG("Database.load: Wczytano str: %u; val: %u\n", sdb.size(), vdb.size());

	return 1;
	}

bool Database::save(const std::string& sname)
	{
	if(sname.length()<2)
		return 0;
	name=sname;
	std::ofstream out(name.c_str(), std::ios::binary|std::ios::trunc);
	unsigned int c=0;
	if(!out)
		{
		LOG_ERROR("Database.save: Nie udalo sie otworzyc pliku \"%s\"\n", name.c_str());
		return 0;
		}
	c=sdb.size();
	out.write(reinterpret_cast<char *>(&c), sizeof(int));
// Zapis const std::string&ow
	for(std::map<std::string, std::string>::iterator it=sdb.begin();
			it!=sdb.end(); ++it)
		{
		unsigned int len;
		len=it->first.length();
		out.write(reinterpret_cast<char *>(&len), sizeof(int));
		out.write(it->first.c_str(), len);
		len=it->second.length();
		out.write(reinterpret_cast<char *>(&len), sizeof(int));
		out.write(it->second.c_str(), len);
		//printf("> str: %s -> %s\n", it->first.c_str(), it->second.c_str());
		}
	if(out.fail())
		{
		LOG_ERROR("Database.save: Zapis nie powiodl sie (str)\n");
		out.close();
		return 0;
		}
// Zapis cyferek
	c=vdb.size();
	out.write(reinterpret_cast<char *>(&c), sizeof(int));
	for(std::map<std::string, int>::iterator it=vdb.begin(); it!=vdb.end();
			++it)
		{
		unsigned int len=it->first.length();
		out.write(reinterpret_cast<char *>(&len), sizeof(int));
		out.write(it->first.c_str(), len);
		int val=it->second;
		out.write(reinterpret_cast<char *>(&val), sizeof(int));
		//printf("> val: %s -> %d\n", it->first.c_str(), it->second);
		}
	if(out.fail())
		{
		LOG_ERROR("Database.save: Zapis nie powiodl sie (str)\n");
		out.close();
		return 0;
		}

	out.close();

	LOG_SUCCESS("Database.save: Zapisano str: %u; val: %u\n", sdb.size(), vdb.size());

	return 1;
	}

void Database::clear()
	{
	sdb.clear();
	vdb.clear();
	}

int Database::getVal(const std::string& name)
	{
	if(vdb.find(name)==vdb.end())
		{
		vdb[name]=0;
		}
	return vdb[name];
	}

std::string Database::getStr(const std::string& name)
	{
	if(sdb.find(name)==sdb.end())
		return "";
	return sdb[name];
	}

void Database::setVal(const std::string& name, int val)
	{
	if(name.length()<1)
		return;
	vdb[name]=val;
	}

void Database::setStr(const std::string& name, const std::string& str)
	{
	if(name.length()<1)
		return;
	sdb[name]=str;
	}
