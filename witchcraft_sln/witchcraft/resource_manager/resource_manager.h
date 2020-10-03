#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <functional>
#include <map>

#include "../../lib/rapidxml/rapidxml.hpp"
#include "../../lib/rapidxml/rapidxml_utils.hpp"
#define XML rapidxml

#include "engine_resource_base.h"
#include "vertex_resource.h"

#include "../message_bus/message_bus.h"

#include "../audio_manager/audio_resource.h"
#include "../render_manager/shader_resource.h"
#include "../render_manager/sprite_atlas_resource.h"
#include "../render_manager/render_manager.h"


// TODO: Create memory budgets for different scenes, systems, and data 

/// @file	resource_manager.h
/// @brief	ResourceManager owns all engine resources, and manages their loading/unloading
///	@note	resource "scope" is used to determine which resources should be loaded when
///	@date	20200926
class ResourceManager : public EngineObjectBase
{
private:
protected:

	int current_scope = 0;
	unsigned int resource_count = 0;

	// consider using EASTL::invasive vector, instead of regular vector here
	std::map<unsigned, std::vector<std::unique_ptr<EngineResourceBase>>> resource_map;

	/// brief: places a successfully parsed xml resource inside the resource map, if it isn't already there
	EngineResourceBase * register_resource(std::unique_ptr<EngineResourceBase> & resource);

	/// brief: parses the top node of an xml file for version information
	char * determine_version(XML::xml_node<> const & xml) const;

	/// brief: parses xml file, where version information is unknown
	EngineResourceBase * parse_file_version__unknown(XML::xml_node<> const & node);

	/// brief: parses first generation witchcraft xml file
	EngineResourceBase * parse_file_version__010(XML::xml_node<> const & node);

	/// brief: parse block for resources, and register each resource
	void process_xml_audio_block(XML::xml_node<> const & audio);
	std::unique_ptr<EngineResourceBase> build_sound_resource_from_xml(XML::xml_node<> const & node);

	// these parse a thing and emit one file
	std::unique_ptr<EngineResourceBase> build_render_resource_from_xml(XML::xml_node<> const & xml);
	std::unique_ptr<EngineResourceBase> build_vertex_resource_from_xml(XML::xml_node<> const & xml);
	std::unique_ptr<EngineResourceBase> build_shader_resource_from_xml(XML::xml_node<> const & xml);


	//std::unique_ptr<EngineResourceBase> load_animation_resource_from_xml(XML::xml_node<> const & xml);
	
	std::vector<Animation2D> parse_embedded_sprite_animations(XML::xml_node<> const & xml);
	Animation2D parse_one_embedded_sprite_animation(XML::xml_node<> const & xml);

	MessageBus * message_bus = nullptr;
	void handle_message(Message m);
	unsigned int resource_channel_id = 0;
	unsigned int render_channel_id   = 0;
	unsigned int engine_channel_id   = 0;

public:

	EngineResourceBase * load_from_xml_file(std::string const & file);

	/// brief: returns a non-owning pointer to resource; searches local scope, then global	/// brief: returns a non-owning pointer to resource; searches local scope, then global
	EngineResourceBase * find_resource(unsigned ID, int scope);
	/// brief: returns a non-owning pointer to resource; searches local scope, then global	/// brief: returns a non-owning pointer to resource; searches local scope, then global
	EngineResourceBase * find_resource(char const * name, int scope);

	// clears all resources and scopes
	void empty_cache();

	
	// sets which scene scope is considered "active"
	bool set_current_scope(unsigned int scope);

	ResourceManager();
	ResourceManager(MessageBus * mb);

	// Simple getters
	int get_current_scope() const;
	unsigned int get_resource_count() const;
};


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
