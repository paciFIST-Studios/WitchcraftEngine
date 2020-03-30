#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "../../lib/rapidxml/rapidxml.hpp"
#include "../../lib/rapidxml/rapidxml_utils.hpp"
#define XML rapidxml

#include "../engine/engine_object.h"
#include "resource.h"

#include "../render_manager/animation_resource.h"
#include "../render_manager/2d_render_manager.h"

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

	std::weak_ptr<c2DRenderManager> _render_manager;

	// a std::map, whose keys are <unsigned int, std::list<cResource*>>
	RESOURCE_MAP_TYPE _resource_map;


public:

	// find resource by id.  retun null if not found
	cResource * find_resource_by_id(unsigned int UID);

	// clears all resources and scopes
	void empty_cache();

	// loads resources from xml
	bool load_from_xml_file(std::string const & file);

	// sets the current scope.  Depends on current scene
	bool set_current_scope(unsigned int scope);

	// Simple getters
	int get_current_scope() const;
	unsigned int get_resource_count() const;

	cResourceManager();

};

// static void create_config_files()
namespace witchcraft
{
	namespace configuration
	{
		static void create_config_files()
		{
			std::list<std::string> config_files = {
					  witchcraft::configuration::witchcraft_cfg
					, witchcraft::configuration::birds_asset
					, witchcraft::configuration::buddha_asset
					, witchcraft::configuration::person_asset
			};

			for (auto file_path : config_files)
			{
				if (utility::file_exists(file_path))
					continue;

				std::string config_data;
				if (file_path == witchcraft::configuration::birds_asset)
				{
					config_data = witchcraft::configuration::birds_asset_file_content;
				}
				else if (file_path == witchcraft::configuration::buddha_asset)
				{
					config_data = witchcraft::configuration::buddha_asset_file_content;
				}
				else if (file_path == witchcraft::configuration::person_asset)
				{
					config_data = witchcraft::configuration::person_asset_file_content;
				}
				else if (file_path == witchcraft::configuration::witchcraft_cfg)
				{
					config_data = witchcraft::configuration::witchcraft_cfg_file_content;
				}

				std::ofstream cfg_file;
				cfg_file.open(file_path);
				cfg_file << config_data;
				cfg_file.close();
				PLOGV << "File created: " << file_path;
			}
		}
	}
}


#endif // RESOURCE_MANAGER_H
