#include "resource_manager.h"

ResourceManager::ResourcePtr ResourceManager::build_render_resource_from_xml(XML::xml_node<> const & xml)
{
	// some default values
	unsigned int	resource_id		= uninit::UINT;
	unsigned int	resource_scope	= uninit::UINT;
	std::string		file_name		= std::string(uninit::CSTRING);

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
		else if (attribute_name == witchcraft::xml::resource_scope)
		{
			resource_scope = std::stoi(attribute_value);
		}
	}

	PLOGV << witchcraft::log_strings::resource_manager_meta_load << file_name;

	// NOTE: We're making a qRenderResource, and then moving it to a qResource ptr
	std::unique_ptr<qResource> resource = std::make_unique<qRenderResource>(
		  resource_id
		, resource_scope
		, file_name
		);

	return std::move(resource);
}

ResourceManager::ResourcePtr ResourceManager::load_animation_resource_from_xml(XML::xml_node<> const & xml)
{
	unsigned int				resource_id				= uninit::UINT;
	unsigned int				resource_scope			= uninit::UINT;
	unsigned int				animation_timing_ms		= uninit::UINT;
	std::vector<unsigned int>	frame_index_sequence;
	std::string					animation_name			= std::string(uninit::CSTRING);

	for (XML::xml_attribute<> * element_attribute = xml.first_attribute();
		element_attribute;
		element_attribute = element_attribute->next_attribute()
	)
	{
		std::string attr_name = element_attribute->name();
		std::string attr_value = element_attribute->value();

		if (attr_name == witchcraft::xml::uuid)
		{
			resource_id = std::stoi(attr_value);
		}
		else if (attr_name == witchcraft::xml::resource_scope)
		{
			resource_scope = std::stoi(attr_value);
		}
		else if (attr_name == witchcraft::xml::resource_name)
		{
			animation_name = attr_value;
		}
		else if (attr_name == witchcraft::xml::two_d_animation_timing_ms)
		{
			animation_timing_ms = std::stoi(attr_value);
		}
		else if (attr_name == witchcraft::xml::two_d_animation_sequence)
		{
			frame_index_sequence.push_back(1);
			// split into a vector, knowing the delimiter
			// todo: move this to the string literal file?
			std::string const delimiters = " ";

			auto sv = utility::tokenize_string(attr_value, delimiters);
			for (auto element : sv)
			{
				frame_index_sequence.push_back
				(
					atoi(element.c_str())
				);
			}
		}
	}

	auto anim = Animation2D(animation_name, frame_index_sequence, animation_timing_ms);

	// NOTE: there is a cast to qResource
	std::unique_ptr<qResource> resource = std::make_unique<AnimationResource>(anim);

	return std::move(resource);
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
					else if (attributeValue == "2d_animation")
					{
						resource = load_animation_resource_from_xml(*child);
						break;
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
