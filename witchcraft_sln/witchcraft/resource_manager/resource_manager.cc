#include "resource_manager.h"

// returns a NON-OWNING ptr
cResource * cResourceManager::find_resource_by_id(unsigned int UID)
{
	if (_resource_count == 0)
		return nullptr;
	
	// iterate through all of the scene ids
	for (auto&& resource_kvp : _resource_map)
	{
		// iterate through the vector associated w/ each id
		for (auto&& element_unique_ptr : (resource_kvp.second))
		{
			auto element = element_unique_ptr.get();
			if (element->_resource_id == UID)
			{
				return element;
			}
		}
	}
	
	// we went through all existing resource without finding a match
	return nullptr;
}


void cResourceManager::empty_cache()
{
	if (_resource_count == 0)
		return;
	
	// look through scene ids
	for (auto&& resource_kvp : _resource_map)
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
	
	_resource_map.clear();
}


bool cResourceManager::load_from_xml_file(std::string Filename)
{
	if (utility::file_exists(Filename) == false)
		return false;

	XML::file<> config_file(Filename.c_str());
	XML::xml_document<> doc;
	doc.parse<0>(config_file.data());

	// Top node is also called the "resource tree"
	XML::xml_node<> * top_node = doc.first_node("resources");

	if (top_node)
	{
		// enumerate objects
		for (XML::xml_node<> * child = top_node->first_node(); child; child = child->next_sibling())
		{
			std::unique_ptr<cResource> resource = nullptr;

			// for each object, enumerate the attributes it contains
			for (XML::xml_attribute<> * childAttribute = child->first_attribute(); 
				childAttribute; 
				childAttribute = childAttribute->next_attribute()
			)
			{
				std::string attributeName = childAttribute->name();
				std::string attributeValue = childAttribute->value();

				// check resourece type
				if (attributeName == "type")
				{
					// We will allow resource managers to implement their own derived
					// versions of cResource.  Those managers will create the resource,
					// and then give us a unique_ptr<cResource> pointer back, and this 
					// scope will need to add the cResource pointer to the resource list.
					if (attributeValue == "graphic")
					{
						resource = _render_manager->load_resource_from_xml(*child);
					}
					else if (attributeValue == "audio")
					{
						// resource = _audio_manager->load_resource_from_xml(child);
					}
					else if (attributeValue == "text")
					{
						// resource = _config_manager->load_resource_from_xml(child);
					}
				}
			}

			if (resource)
			{	
				// we must use std::move to change ownership of the unique_ptr
				_resource_map[resource->_scope].push_back(std::move(resource));
				_resource_count++;
			}
		}

		return true;
	}
	
	return false;
}


// WARN: Must be called for each scene change
void cResourceManager::set_current_scope(unsigned int Scope)
{
	if (_resource_count == 0)
		return;
	
	// unload old scope, but not global
	if (_current_scope != 0)
	{
		for (auto&& element_unique_ptr : _resource_map[_current_scope])
		{
			auto element = element_unique_ptr.get();
			element->unload();
		}
	}
	
	_current_scope = Scope;
	
	for (auto&& element_unique_ptr : _resource_map[_current_scope])
	{
		auto element = element_unique_ptr.get();
		element->load();
	}
}


cResourceManager::cResourceManager() 
	: _resource_count(0)
	, _current_scope(RESOURCE_GLOBAL_SCOPE)
{}
