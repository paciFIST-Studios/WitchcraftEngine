#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <functional>
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

#include "../render_manager/SDL2_2D_render_manager.h"

#include "../utility/utility.h"

<<<<<<< HEAD
<<<<<<< HEAD:Witchcraft/Witchcraft/resource_manager/resource_manager.h
<<<<<<< Updated upstream:Witchcraft/Witchcraft/resource_manager/resource_manager.h
#define GLOBAL_SCOPE 0
=======
#define RESOURCE_MAP_TYPE std::map<unsigned int, std::list<cResource*>>
=======
>>>>>>> render_manager_implementation:witchcraft_sln/witchcraft/resource_manager/resource_manager.h
=======
#define RESOURCE_MAP_TYPE std::map<unsigned int, std::vector<std::unique_ptr<cResource>>>
>>>>>>> master
#define RESOURCE_GLOBAL_SCOPE 0


// TODO: Create memory budgets for different scenes, systems, and data 

<<<<<<< HEAD
<<<<<<< HEAD:Witchcraft/Witchcraft/resource_manager/resource_manager.h
>>>>>>> Stashed changes:witchcraft_sln/witchcraft/resource_manager/resource_manager.h
=======
>>>>>>> render_manager_implementation:witchcraft_sln/witchcraft/resource_manager/resource_manager.h
=======
>>>>>>> master

// resource manager, to manage the resource objects
class cResourceManager : public cEngineObject
{
private:
protected:

	// Also the "scene id" of the current scene
	unsigned int _current_scope = 0;

	// total resources managed
	unsigned int _resource_count = 0;

	cSDL2RenderManager * _render_manager = nullptr;

	// a std::map, whose keys are <unsigned int, std::list<cResource*>>
	RESOURCE_MAP_TYPE _resource_map;

public:

	const int GLOBAL_SCOPE_ID = 0;

	// find resource by id.  retun null if not found
	cResource* find_resource_by_id(unsigned int UID);

	// clears all resources and scopes
	void empty_cache();

	// loads resources from xml
	bool load_from_xml_file(std::string Filename);

	// sets the current scope.  Depends on current scene
	bool set_current_scope(unsigned int Scope);

	// Simple getters
	inline int get_current_scope() const { return _current_scope; }
	inline unsigned int get_resource_count() const { return _resource_count; }

	cResourceManager();

	bool create_config_files()
	{
		//ULOG("\ncreate_config_files()\n");

		std::list<std::string> config_files = {
 			  std::string("witchcraft.cfg")
			, std::string("asset/birds.asset")
			, std::string("asset/buddha.asset")
			, std::string("asset/neko.asset")
			, std::string("asset/person.asset")
			, std::string("asset/whirlwind.asset")
		};
		
		for (auto cfg_itr = config_files.begin(); cfg_itr != config_files.end(); cfg_itr++)
		{
			if (utility::file_exists(*cfg_itr))
			{
				//std::ostringstream os;
				//os << "\tskipping file: " << *cfg_itr << "\n";
				//ULOG(os.str());
				continue;
			}
<<<<<<< HEAD
<<<<<<< Updated upstream:Witchcraft/Witchcraft/resource_manager/resource_manager.h
				
			std::string config_data = "<resources><resource UID=\"0\" type=\"text\" filename=\"witchcraft.cfg\" scenescope=\"0\">Witchcraft</resource></resources>";

			std::ofstream cfg_file;
			cfg_file.open(*cfg_itr);
<<<<<<< HEAD:Witchcraft/Witchcraft/resource_manager/resource_manager.h
			cfg_file << "\0";
=======
=======
>>>>>>> master

			std::string file_path = static_cast<std::string>(*cfg_itr);

			std::string config_data;
			if (file_path == "asset/birds.asset")
			{
				config_data = "<resources><resource UID=\"1\" type=\"graphic\" filename=\"asset/birds.png\" scenescope=\"1\">Winter Birds</resource></resources>";
			}
			else if (file_path == "asset/buddha.asset")
			{
				config_data = "<resources><resource UID=\"2\" type=\"graphic\" filename=\"asset/buddha.png\" scenescope=\"0\">Buddha</resource></resources>";
			}
			else if (file_path == "asset/person.asset")
			{
				config_data = "<resources><resource UID=\"4\" type=\"graphic\" filename=\"asset/person.png\" scenescope=\"2\">Person</resource></resources>";
			}
			else if (file_path == "witchcraft.cfg")
			{
				 config_data = "<resources><resource UID=\"0\" type=\"text\" filename=\"witchcraft.cfg\" scenescope=\"0\">Witchcraft</resource></resources>";
			}

			std::ofstream cfg_file;
			cfg_file.open(file_path);
			cfg_file << config_data;
<<<<<<< HEAD
>>>>>>> Stashed changes:witchcraft_sln/witchcraft/resource_manager/resource_manager.h
=======
			cfg_file << config_data;
>>>>>>> render_manager_implementation:witchcraft_sln/witchcraft/resource_manager/resource_manager.h
=======
>>>>>>> master
			cfg_file.close();

			if (utility::file_exists(file_path))
			{
				//std::ostringstream os;				
				//os << "\tfile created: " << file_path << "\n";
				//ULOG(os.str());
			}
		}
		
		return true;
	}
};

#endif // RESOURCE_MANAGER_H
