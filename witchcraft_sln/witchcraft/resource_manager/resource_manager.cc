#include "resource_manager.h"

ResourceManager::ResourcePtr ResourceManager::build_render_resource_from_xml(XML::xml_node<> const & xml)
{
	// some default values
	unsigned int	resource_id		= uninit::UINT;
	unsigned int	resource_scope	= uninit::UINT;
	std::string		file_name		= std::string(uninit::CSTRING);
	std::string		friendly_name = std::string(uninit::CSTRING);

	bool is_sprite_atlas = false;
	unsigned int sprite_atlas_tile_width  = uninit::UINT;
	unsigned int sprite_atlas_tile_height = uninit::UINT;

	for (XML::xml_attribute<> * element_attribute = xml.first_attribute();
		element_attribute;
		element_attribute = element_attribute->next_attribute()
	)
	{
		std::string attribute_name  = element_attribute->name();
		std::string attribute_value = element_attribute->value();

		if (attribute_name == witchcraft::xml::uuid)
		{
			// stoi stands for string-to-integer, and is used for
			// parsing a string to an int
			resource_id = std::stoi(attribute_value);
		}
		else if (attribute_name == witchcraft::xml::file_name)
		{
			file_name = attribute_value;
		}
		else if (attribute_name == witchcraft::xml::resource_name)
		{
			friendly_name = attribute_value;
		}
		else if (attribute_name == witchcraft::xml::resource_scope)
		{
			resource_scope = std::stoi(attribute_value);
		}
		else if (attribute_name == witchcraft::xml::sprite_atlas)
		{
			// NOTE: format is: sprite_atlas="w,h"
			// if we find this string, and it's formatted correctly, then:
			// we count it as a sprite atlas
			// we extract the tile size from the string
			auto atlas_vec = utility::tokenize_string(attribute_value, witchcraft::xml::delimiter);
			if (atlas_vec.size() == 2)
			{
				sprite_atlas_tile_width  = std::stoi(atlas_vec[0]);
				sprite_atlas_tile_height = std::stoi(atlas_vec[1]);
				is_sprite_atlas = true;
			}
		}
	}

	PLOGV << witchcraft::log_strings::resource_manager_meta_load << file_name;

	// note: we're going to make something derived from qResource
	std::unique_ptr<qResource> resource;

	if (is_sprite_atlas)
	{
		auto sar = std::make_unique<SpriteAtlasResource>(
			  resource_id
			, resource_scope
			, file_name
			, sprite_atlas_tile_width
			, sprite_atlas_tile_height
			);

		auto embedded_animations = parse_embedded_sprite_animations(xml);
		for (auto&& anim : embedded_animations)
		{
			sar->add_animation(anim.name, anim);
			PLOGV << witchcraft::log_strings::resource_manager_meta_load 
				<< "{ atlas=\"" << friendly_name << "\"  anim=\"" << anim.name << "\" }";
				// NOTE: in CPP20 we can do the following, with a more python style
				// << std::format("atlas=\"{0}\}    anim=\"{1}\"", friendly_name, anim.name);
		}

		resource = std::move(sar);
	}
	else // create a basic render resource
	{
		auto rr = std::make_unique<qRenderResource>(resource_id, resource_scope, file_name);
		resource = std::move(rr);
	}
	
	return std::move(resource);
}


std::vector<Animation2D> ResourceManager::parse_embedded_sprite_animations(XML::xml_node<> const & xml)
{
	std::vector<Animation2D> result;

	for (XML::xml_node<> * child = xml.first_node(); child; child = child->next_sibling())
	{
		auto anim = parse_one_embedded_sprite_animation(*child);
		result.emplace_back(anim);
	}

	return result;
}

Animation2D ResourceManager::parse_one_embedded_sprite_animation(XML::xml_node<> const & xml)
{
	std::string					animation_name			= std::string(uninit::CSTRING);
	unsigned int				animation_ms_per_frame  = uninit::UINT;
	std::vector<unsigned int>	frame_index_sequence    ;

	for (XML::xml_attribute<> * element_attribute = xml.first_attribute();
		element_attribute;
		element_attribute = element_attribute->next_attribute()
		)
	{
		std::string attr_name = element_attribute->name();
		std::string attr_value = element_attribute->value();

		if (attr_name == witchcraft::xml::resource_name)
		{
			animation_name = attr_value;
		}
		else if (attr_name == witchcraft::xml::animation_2d_ms_per_frame)
		{
			animation_ms_per_frame = std::stoi(attr_value);
		}
		else if (attr_name == witchcraft::xml::animation_2d_sequence)
		{
			auto sv = utility::tokenize_string(attr_value, witchcraft::xml::delimiter);
			for (auto element : sv)
			{
				frame_index_sequence.push_back(std::stoi(element));
			}
		}
	}
	auto result = Animation2D(
		  animation_name
		, animation_ms_per_frame
		, frame_index_sequence
	);
	return result;
}


// returns a NON-OWNING ptr
qResource * ResourceManager::find_resource_by_id(unsigned int UUID)
{
	if (resource_count == 0){ return nullptr; }

	if (UUID == -1)
	{ 
		PLOGE << "NO RESOURCE FOUND!"; 
		return nullptr;
	}


	// iterate through all of the scene ids
	for (auto&& resource_kvp : resource_map)
	{
		// iterate through the vector associated w/ each id
		for (auto&& element_unique_ptr : (resource_kvp.second))
		{
			auto element = element_unique_ptr.get();
			if (element->get_resource_id() == UUID)
			{
				return element;
			}
		}
	}
	
	// we went through all existing resource without finding a match
	return nullptr;
}

void ResourceManager::empty_cache()
{
	if (resource_count == 0)
		return;
	
	// look through scene ids
	for (auto&& resource_kvp : resource_map)
	{
		// and the resource lists associated with each id
		for (auto&& element_unique_ptr : resource_kvp.second)
		{
			auto element = element_unique_ptr.get();
			element->unload();

			// reset destroys the held ptr, release changes ownership
			element_unique_ptr.reset();
		}
	
		resource_kvp.second.clear();
	}
	
	resource_map.clear();
	resource_count = 0;
	current_scope = witchcraft::configuration::global_resource_scope;
}

int ResourceManager::load_from_xml_file(std::string const & file)
{
	if (false == utility::file_exists(file))
	{
		PLOGE << "ERROR: FILE DOES NOT EXIST; path: \"" << file << "\"";
		return -1;
	}

	XML::file<> config_file(file.c_str());
	XML::xml_document<> doc;
	doc.parse<0>(config_file.data());

	// Top node is also called the "resource tree"
	XML::xml_node<> * top_node = doc.first_node(witchcraft::xml::resource_list.c_str());

	if (top_node)
	{
		// enumerate objects
		for (XML::xml_node<> * child = top_node->first_node(); child; child = child->next_sibling())
		{
			std::unique_ptr<qResource> resource = nullptr;

			// for each object, enumerate the attributes it contains
			for (XML::xml_attribute<> * childAttribute = child->first_attribute(); 
				childAttribute; 
				childAttribute = childAttribute->next_attribute()
			)
			{
				std::string attributeName = childAttribute->name();
				std::string attributeValue = childAttribute->value();

				// check resourece type
				if (attributeName == witchcraft::xml::resource_type)
				{
					// We will allow/force resource managers to implement their own derived
					// versions of qResource.  Those managers will create the resource,
					// and then give us a unique_ptr<qResource> pointer back, and this 
					// scope will need to add the qResource pointer to the resource list.
					if (attributeValue == "graphic")
					{
						resource = build_render_resource_from_xml(*child);
						break;
					}
					else if (attributeValue == "audio")
					{
						// resource = _audio_manager->load_resource_from_xml(child);
						// break;
					}
					else if (attributeValue == "text")
					{
						// resource = _config_manager->load_resource_from_xml(child);
						// break;
					}
				}
			}

			if (resource)
			{	
				// do not add duplicates of the same file
				if (find_resource_by_id(resource->get_resource_id()))
					// TODO: figure out duplicates w/o completely loading the file
					return resource->get_resource_id();

				auto id = resource->get_resource_id();
				// we must use std::move to change ownership of the unique_ptr
				resource_map[resource->get_scope_id()].push_back(std::move(resource));
				resource_count++;
				return id;
			}
		}
	}
	
	return -1;
}

// WARN: Must be called for each scene change
bool ResourceManager::set_current_scope(unsigned int Scope)
{
	// You cannot change scope, until a global resource is loaded
	if (resource_count == 0)
		return false;
	
	// unload old scope, but not global
	if (current_scope != 0)
	{
		for (auto& element_unique_ptr : resource_map[current_scope])
		{
			auto element = element_unique_ptr.get();
			element->unload();
		}
	}
	
	current_scope = Scope;
	
	for (auto&& element_unique_ptr : resource_map[current_scope])
	{
		auto element = element_unique_ptr.get();
		element->load();
	}

	return true;
}

ResourceManager::ResourceManager()
	: resource_count(0)
	, current_scope(witchcraft::configuration::global_resource_scope)
{}

int ResourceManager::get_current_scope() const
{
	return current_scope;
}

unsigned int ResourceManager::get_resource_count() const
{
	return resource_count;
}
