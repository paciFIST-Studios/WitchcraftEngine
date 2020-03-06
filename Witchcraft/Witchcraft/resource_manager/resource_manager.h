#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <list>
#include <map>
#include <string>

#include "../../lib/rapidxml/rapidxml.hpp"
#include "../../lib/rapidxml/rapidxml_utils.hpp"
#define XML rapidxml


#include "../engine/exception.h"
#include "../engine/engine.h"
#include "../utility/utility.h"


typedef enum {
	  RESOURCE_NULL = 0
	, RESOURCE_GRAPHIC = 1
	, RESOURCE_MOVIE = 2
	, RESOURCE_AUDIO = 3
	, RESOURCE_TEXT = 4
} RESOURCE_TYPE;

#define GLOBAL_SCOPE 0


// ----------------------------------------

#define SAFE_DELETE(a) { delete(a); (a)=nullptr; }

// The cResource class represents a resource object, which is managed by a resource manager
// to be inhereted from, by other objects
class cResource : public cEngineObject
{
private:
protected:
public:
	unsigned int _resource_id;
	unsigned int _scope;
	std::string _file_name;
	RESOURCE_TYPE _type;

	// to overload
	virtual ~cResource() {}
	virtual void load() {}
	virtual void unload() {}
	
	inline cResource()
	{
		_resource_id = _scope = 0;
		_type = RESOURCE_NULL;
	}

};


// ----------------------------------------

// resource manager, to manage the resource objects
class cResourceManager : public cEngineObject
{
private:
protected:

	// current scope
	unsigned int _current_scope;

	// total resources managed
	unsigned int _resource_count;

public:

	// map<scope, resource_list>
	std::map<unsigned int, std::list<cResource*>> _resource_map;

	// find resource by id.  retun null if not found
	cResource* find_resource_by_id(unsigned int UID);

	// clears all resources and scopes
	void clear_all();

	// loads resources from xml
	bool load_from_xml_file(std::string Filename);

	// sets the current scope.  Depends on current scene
	void set_current_scope(unsigned int Scope);

	const unsigned int get_resource_count(){return _resource_count;}
	

	bool create_config_files()
	{
		std::cout << "create_config_files()\n";

		std::list<std::string> config_files = {
			std::string("witchcraft.cfg")
		};

		
		for (auto cfg_itr = config_files.begin(); cfg_itr != config_files.end(); cfg_itr++)
		{
			if (utility::file_exists(*cfg_itr))
			{
				std::cout << "\tskipping file: " << *cfg_itr << "\n";
				continue;
			}
				
			std::ofstream cfg_file;
			cfg_file.open(*cfg_itr);
			cfg_file << "\0";
			cfg_file.close();

			if (utility::file_exists(*cfg_itr))
			{
				std::cout << "\tfile created: " << *cfg_itr << "\n";
			}
		}
		
		return true;
	}

	//bool parse_config_files()
	//{
	//	// due to how rapidxml works, we start by loading all of this stuff into memory
	//
	//	std::list<char const *> filenames = {
	//		"config.xml"
	//	};
	//
	//	for (auto file_itr = filenames.begin(); file_itr != filenames.end(); file_itr++)
	//	{
	//		try
	//		{
	//			XML::file<> config_file(*file_itr);
	//		}
	//		catch(cException ex)
	//		{
	//			return false;
	//		}
	//
	//		cResource * resource = nullptr;
	//		resource->_scope = GLOBAL_SCOPE;
	//		resource->_type = RESOURCE_TYPE::RESOURCE_TEXT;
	//		resource->_file_name = *file_itr;
	//
	//
	//		_resource_map[GLOBAL_SCOPE];
	//	}
	//
	//	return true;
	//}
	//
	inline cResourceManager() 
	{
		_current_scope = _resource_count = 0;
		
		create_config_files();
	}

};





#endif // RESOURCE_MANAGER_H
