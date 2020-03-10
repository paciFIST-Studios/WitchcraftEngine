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
#include "resource.h"

#include "../utility/utility.h"

#define RESOURCE_GLOBAL_SCOPE 0


// TODO: Create memory budgets for different scenes, systems, and data 


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
	void empty_cache();

	// loads resources from xml
	bool load_from_xml_file(std::string Filename);

	// sets the current scope.  Depends on current scene
	void set_current_scope(unsigned int Scope);

	inline int get_current_scope() { return _current_scope; }

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
				
			std::string config_data = "<resources><resource UID=\"0\" type=\"text\" filename=\"witchcraft.cfg\" scenescope=\"0\">Witchcraft</resource></resources>";

			std::ofstream cfg_file;
			cfg_file.open(*cfg_itr);
			cfg_file << config_data;
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
	//		resource->_scope = RESOURCE_GLOBAL_SCOPE;
	//		resource->_type = RESOURCE_TYPE::RESOURCE_TEXT;
	//		resource->_file_name = *file_itr;
	//
	//
	//		_resource_map[RESOURCE_GLOBAL_SCOPE];
	//	}
	//
	//	return true;
	//}
	//
	inline cResourceManager() 
	{
		_current_scope = _resource_count = 0;
		
		//create_config_files();
	}

};

#endif // RESOURCE_MANAGER_H
