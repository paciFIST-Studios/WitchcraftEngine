#include "resource_manager.h"

cResource * cResourceManager::find_resource_by_id(unsigned int UID)
{
	if (_resource_count == 0)
		return nullptr;

	// search scopes for correct scope
	std::map<unsigned int, std::list<cResource*>>::iterator element_itr;
	for (element_itr = _resource_map.begin(); element_itr != _resource_map.end(); element_itr++)
	{
		// until we run out
		if (!(*element_itr).second.empty())
		{
			// search elements(resources) of this scope
			std::list<cResource*>::iterator list_itr;
			for (list_itr = (*element_itr).second.begin(); list_itr != (*element_itr).second.end(); list_itr++)
			{
				// check for id match
				if ((*list_itr)->_resource_id == UID)
					return (*list_itr);
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

	std::map<unsigned int, std::list<cResource*>>::iterator element_itr;
	
	// look through scopes
	for (element_itr = _resource_map.begin(); element_itr != _resource_map.end(); element_itr++)
	{
		if (!(*element_itr).second.empty())
		{
			std::list<cResource*>::iterator list_itr;

			// search the resources for this specific scope
			for (list_itr = (*element_itr).second.begin(); list_itr != (*element_itr).second.end(); list_itr++)
			{
				// delete resrouce object
				(*list_itr)->unload();
				SAFE_DELETE(*list_itr);
			}

			(*element_itr).second.clear();
		}
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
			cResource * resource = nullptr;

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
					// and then give us a cResource pointer back, and this scope will need
					// to add the cResource pointer to the resource list.

					if (attributeValue == "graphic")
					{
						resource = _render_manager->load_resource_from_xml(child);
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

				// skipped, unless loading completes in a prior step
				if (resource)
				{
					if (attributeName == "UID")
					{
						resource->_resource_id = atoi(attributeValue.c_str());
					}
					else if (attributeName == "filename")
					{
						resource->_file_name = attributeValue;
					}
					else if (attributeName == "scenescope")
					{
						resource->_scope = atoi(attributeValue.c_str());
					}
				}
			}

			if (resource)
			{	
				_resource_map[resource->_scope].push_back(resource);
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
		std::list<cResource*>::iterator list_itr;

		for (list_itr = _resource_map[_current_scope].begin();
			list_itr != _resource_map[_current_scope].end();
			list_itr++)
		{
			// note: also calling delete here would kill our cResource obj
			// and that would delete the metadata, which we would use to
			// reload this asset, if the scene becomes active again.
			(*list_itr)->unload();
		}
	}

	_current_scope = Scope;

	// load all resources for new scope
	std::list<cResource*>::iterator list_itr;
	for (list_itr = _resource_map[_current_scope].begin();
		list_itr != _resource_map[_current_scope].end();
		list_itr++)
	{
		(*list_itr)->load();
	}
}

cResourceManager::cResourceManager() 
	: _resource_count(0)
	, _current_scope(RESOURCE_GLOBAL_SCOPE)
{}
