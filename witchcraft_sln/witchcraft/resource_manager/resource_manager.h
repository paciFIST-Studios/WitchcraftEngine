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

#include <plog/Log.h>

#include "../engine/exception.h"
#include "../engine/engine.h"
#include "resource.h"

#include "../render_manager/animation_resource.h"
#include "../render_manager/2d_render_manager.h"

#include "../utility/utility.h"

#define RESOURCE_MAP_TYPE std::map<unsigned int, std::vector<std::unique_ptr<cResource>>>
#define RESOURCE_GLOBAL_SCOPE 0


// TODO: Create memory budgets for different scenes, systems, and data 


// resource manager, to manage the resource objects
class cResourceManager : public cEngineObject
{
private:
protected:

	// Also the "scene id" of the current scene
	unsigned int _current_scope = 0;

	// total resources managed
	unsigned int _resource_count = 0;

	c2DRenderManager * _render_manager = nullptr;

	// a std::map, whose keys are <unsigned int, std::list<cResource*>>
	RESOURCE_MAP_TYPE _resource_map;


public:

	static int constexpr GLOBAL_SCOPE_ID = 0;

	// find resource by id.  retun null if not found
	cResource * find_resource_by_id(unsigned int UID);

	// clears all resources and scopes
	void empty_cache();

	// loads resources from xml
	bool load_from_xml_file(std::string const & filename);

	// sets the current scope.  Depends on current scene
	bool set_current_scope(unsigned int scope);

	// Simple getters
	inline int get_current_scope() const { return _current_scope; }
	inline unsigned int get_resource_count() const { return _resource_count; }

	cResourceManager();

	bool create_config_files()
	{
		std::list<std::string> config_files = {
 			  std::string("witchcraft.cfg")
			, std::string("asset/birds.asset")
			, std::string("asset/buddha.asset")
			, std::string("asset/neko.asset")
			, std::string("asset/person.asset")
			, std::string("asset/whirlwind.asset")
		};

		for (auto file_path : config_files)
		{
			if (utility::file_exists(file_path))
				continue;
		
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
				//config_data = "<resources><resource UID=\"4\" type=\"graphic\" filename=\"asset/person.png\" scenescope=\"2\"><></></resource></resources>";
				config_data = "<resources>\n\t<resource UID=\"4\" type=\"graphic\" filename=\"asset / person.png\" scenescope=\"2\"></resource>\n\t<resource UID=\"100\" type=\"2d_animation\" name=\"idle\" timing_ms=\"250\" sequence=\"1 2 3 1 2 3 1 2 3 3 3 4 4\"></resource>\n</resources>";
			}
			else if (file_path == "witchcraft.cfg")
			{
				 config_data = "<resources><resource UID=\"0\" type=\"text\" filename=\"witchcraft.cfg\" scenescope=\"0\">Witchcraft</resource></resources>";
			}

			std::ofstream cfg_file;
			cfg_file.open(file_path);
			cfg_file << config_data;
			cfg_file.close();
			PLOGV << "File created: " << file_path;
		}
		
		return true;
	}
};

#endif // RESOURCE_MANAGER_H
