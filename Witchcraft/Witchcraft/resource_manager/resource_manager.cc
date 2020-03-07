#include "resource_manager.h"

cResource * cResourceManager::find_resource_by_id(unsigned int UID)
{
	if (_resource_count == 0)
		return nullptr;

	std::map<unsigned int, std::list<cResource*>>::iterator element_itr;

	// search scopes for correct scope
	for (element_itr = _resource_map.begin(); element_itr != _resource_map.end(); element_itr++)
	{
		if (!(*element_itr).second.empty())
		{
			std::list<cResource*>::iterator list_itr;

			// search elements(resources) of this scope
			for (list_itr = (*element_itr).second.begin(); list_itr != (*element_itr).second.end(); list_itr++)
			{
				// check for id match
				if ((*list_itr)->_resource_id == UID)
					return (*list_itr);
			}
		}
	}

	return nullptr;
}

void cResourceManager::clear_all()
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

	// find top node
	XML::xml_node<> * ResourceTree = doc.first_node("resources");

	if (ResourceTree)
	{
		// enumerate objects
		for (XML::xml_node<> * child = ResourceTree->first_node(); child; child = child->next_sibling())
		{
			cResource * resource = nullptr;

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
					// We will allow resource managers to implement their own decendant
					// versions of cResource.  Those managers will create the resource,
					// and then give us a cResource pointer back, and this scope will need
					// to add the cResource pointer to the resourece list.

					if (attributeValue == "graphic")
					{
						// Resourece = g_RenderManager->loadResourceFromXML(Element);
					}
					if (attributeValue == "audio")
					{
						// Resource = g_AudioManager->loadResourceFromXML(Element);
					}
					if (attributeValue == "text")
					{
						//Resource = g_ConfigManager->loadResourceFromXML(Element);
					}
				}

				if (resource)
				{
					if (attributeName == "UID")
					{
						resource->_resource_id = atoi(attributeValue.c_str());
					}
					if (attributeName == "filename")
					{
						resource->_file_name = attributeValue;
					}
					if (attributeName == "scenescope")
					{
						resource->_scope = atoi(attributeValue.c_str());
					}
				}
			}

			if (resource)
			{
				// resource added here
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

	// load new scope
	std::list<cResource*>::iterator list_itr;

	for (list_itr = _resource_map[_current_scope].begin();
		list_itr != _resource_map[_current_scope].end();
		list_itr++)
	{
		(*list_itr)->load();
	}
}

