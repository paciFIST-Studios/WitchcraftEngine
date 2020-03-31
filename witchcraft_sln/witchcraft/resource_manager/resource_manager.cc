#include "resource_manager.h"

// returns a NON-OWNING ptr
cResource * cResourceManager::find_resource_by_id(unsigned int UUID)
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
			if (element->get_resource_id() == UUID)
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
	_resource_count = 0;
	_current_scope = RESOURCE_GLOBAL_SCOPE;
}


bool cResourceManager::load_from_xml_file(std::string const & file)
{
	if (false == utility::file_exists(file))
		return false;

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
				if (attributeName == witchcraft::xml::resource_type)
				{
					// We will allow/force resource managers to implement their own derived
					// versions of cResource.  Those managers will create the resource,
					// and then give us a unique_ptr<cResource> pointer back, and this 
					// scope will need to add the cResource pointer to the resource list.
					if (attributeValue == "graphic")
					{
						//resource = cb_load_render_resource_from_xml(*child);
						resource = _render_manager->load_resource_from_xml(*child);
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
						unsigned int resource_id = uninit::UINT;
						unsigned int resource_scope = uninit::UINT;
						unsigned int animation_timing_ms = uninit::UINT;
						std::vector<unsigned int> frame_index_sequence;
						std::string animation_name = std::string(uninit::CSTRING);

						for (XML::xml_attribute<> * element_attribute = (*child).first_attribute();
							element_attribute;
							element_attribute = element_attribute->next_attribute()
						)
						{
							std::string attr_name = element_attribute->name();
							std::string attr_value = element_attribute->value();

							if (attr_name == witchcraft::xml::uuid)
							{
								resource_id = atoi(attr_value.c_str());
							}
							else if (attr_name == witchcraft::xml::resource_scope)
							{
								resource_scope = atoi(attr_value.c_str());
							}
							else if (attr_name == witchcraft::xml::resource_name)
							{
								animation_name = attr_value;
							}
							else if (attr_name == witchcraft::xml::two_d_animation_timing_ms)
							{
								animation_timing_ms = atoi(attr_value.c_str());
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

						auto anim = c2DSpriteAnimation(animation_name, frame_index_sequence, animation_timing_ms);
						resource = std::make_unique<cAnimationResource>(anim);
						break;
					} // end: 2d_animation
				}
			}

			if (resource)
			{	
				// do not add duplicates of the same file
				if (find_resource_by_id(resource->get_resource_id()))
					// TODO: figure out duplicates w/o completely loading the file
					return false;

				// we must use std::move to change ownership of the unique_ptr
				_resource_map[resource->get_scope_id()].push_back(std::move(resource));
				_resource_count++;
			}
		}

		return true;
	}
	
	return false;
}


// WARN: Must be called for each scene change
bool cResourceManager::set_current_scope(unsigned int Scope)
{
	// You cannot change scope, until a global resource is loaded
	if (_resource_count == 0)
		return false;
	
	// unload old scope, but not global
	if (_current_scope != 0)
	{
		for (auto& element_unique_ptr : _resource_map[_current_scope])
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

	return true;
}


cResourceManager::cResourceManager()
	: _resource_count(0)
	, _current_scope(RESOURCE_GLOBAL_SCOPE)
{}

int cResourceManager::get_current_scope() const
{
	return _current_scope;
}

unsigned int cResourceManager::get_resource_count() const
{
	return _resource_count;
}

//void cResourceManager::set_callback__load_render_resource(c2DRenderManager & render_manager)
//{
//	cb_load_render_resource_from_xml = &(render_manager.load_resource_from_xml);
//}
