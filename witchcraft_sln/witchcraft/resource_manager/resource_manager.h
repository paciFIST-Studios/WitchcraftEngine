#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <functional>
#include <map>
#include <string>

#include "../../lib/rapidxml/rapidxml.hpp"
#include "../../lib/rapidxml/rapidxml_utils.hpp"
#define XML rapidxml

#include "../engine/engine_object.h"
#include "resource.h"

#include "../message_bus/message_bus.h"

#include "../render_manager/sprite_atlas_resource.h"
#include "../render_manager/render_manager.h"


// TODO: Create memory budgets for different scenes, systems, and data 
struct Message;
class MessageBus;

// resource manager, to manage the resource objects
class ResourceManager : public qEngineObject
{
private:
protected:

	// Also the "scene id" of the current scene
	unsigned int current_scope = uninit::UINT;

	// total resources managed
	unsigned int resource_count = 0;

	// a std::map, whose keys are <unsigned int, std::list<qResource*>>
	std::map<unsigned int, std::vector<std::unique_ptr<qResource>>> resource_map;

	std::unique_ptr<qResource> build_render_resource_from_xml(XML::xml_node<> const & xml);
	//ResourcePtr load_animation_resource_from_xml(XML::xml_node<> const & xml);

	std::vector<Animation2D> parse_embedded_sprite_animations(XML::xml_node<> const & xml);
	Animation2D parse_one_embedded_sprite_animation(XML::xml_node<> const & xml);

	MessageBus * message_bus = nullptr;

	void handle_resource_request(Message m);

public:


	// find resource by id.  return nullptr if not found
	qResource * find_resource_by_id(unsigned int UID);

	// clears all resources and scopes
	void empty_cache();

	// loads resources from xml
	int load_from_xml_file(std::string const & file);
	
	// sets which scene scope is considered "active"
	bool set_current_scope(unsigned int scope);

	ResourceManager();
	ResourceManager(MessageBus * mb);

	// Simple getters
	int get_current_scope() const;
	unsigned int get_resource_count() const;
};

// static void create_config_files()
namespace witchcraft
{
	namespace configuration
	{
		static unsigned int const global_resource_scope = 0;

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
