#include "resource_manager.h"

std::unique_ptr<EngineResourceBase> ResourceManager::build_render_resource_from_xml(XML::xml_node<> const & xml)
{
	// some default values
	unsigned int	scope			= 0;
	std::string		filepath		= "";
	std::string		name			= "";

	bool is_sprite_atlas = false;
	unsigned int sprite_atlas_tile_width  = 0;
	unsigned int sprite_atlas_tile_height = 0;

	for (XML::xml_attribute<> * attr = xml.first_attribute(); attr; attr = attr->next_attribute())
	{
		std::string _name  = attr->name();
		std::string _value = attr->value();

		if (_name == witchcraft::xml::NAME)
		{
			// stoi stands for string-to-integer, and is used for
			// parsing a string to an int
			name = _value;
		}
		else if (_name == witchcraft::xml::FILEPATH)
		{
			filepath = _value;
		}
		else if (_name == witchcraft::xml::SCOPE)
		{
			scope = std::stoi(_value);
		}
		else if (_name == witchcraft::xml::sprite_atlas)
		{
			// NOTE: format is: sprite_atlas="w,h"
			// if we find this string, and it's formatted correctly, then:
			// we count it as a sprite atlas
			// we extract the tile size from the string
			auto atlas_vec = utility::tokenize_string(_value, witchcraft::xml::delimiter);
			if (atlas_vec.size() == 2)
			{
				sprite_atlas_tile_width  = std::stoi(atlas_vec[0]);
				sprite_atlas_tile_height = std::stoi(atlas_vec[1]);
				is_sprite_atlas = true;
			}
		}
	}

	PLOGV << witchcraft::log_strings::resource_manager_meta_load << filepath;

	// note: we're going to make something derived from EngineResourceBase
	std::unique_ptr<EngineResourceBase> resource;

	if (is_sprite_atlas)
	{
		auto sar = std::make_unique<SpriteAtlasResource>(
			  name
			, filepath
			, scope
			, sprite_atlas_tile_width
			, sprite_atlas_tile_height
			);

		auto embedded_animations = parse_embedded_sprite_animations(xml);
		for (auto&& anim : embedded_animations)
		{
			sar->add_animation(anim.name, anim);
			PLOGV << witchcraft::log_strings::resource_manager_meta_load 
				<< "{ atlas=\"" << filepath << "\"  anim=\"" << anim.name << "\" }";
				// NOTE: in CPP20 we can do the following, with a more python style
				// << std::format("atlas=\"{0}\}    anim=\"{1}\"", friendly_name, anim.name);
		}

		resource = std::move(sar);
	}
	else // create a basic render resource
	{
		auto rr = std::make_unique<SDLRenderResource>(name, filepath, scope);
		resource = std::move(rr);
	}
	
	return std::move(resource);
}

std::unique_ptr<EngineResourceBase> ResourceManager::build_vertex_resource_from_xml(XML::xml_node<> const & xml)
{
	std::string		name		= "";
	std::string		filepath	= "";
	EResourceType	type		= EResourceType::VERTEX_LIST_QUAD;
	unsigned int	scope		= 0;
	std::vector<float> verts;
	std::vector<int> indicies;

	bool record_vertex  = false;
	bool record_texture = false;
	bool record_color   = false;

	int vertex_stride  = 0;
	int vertex_offset  = 0;
	int texture_stride = 0;
	int texture_offset = 0;
	int color_stride   = 0;
	int color_offset   = 0;

	for (XML::xml_attribute<> * attr = xml.first_attribute(); attr; attr = attr->next_attribute())
	{
		std::string _name = attr->name();
		std::string _value = attr->value();

		if (_name == witchcraft::xml::NAME)
		{
			name = _value;
		}
		else if (_name == witchcraft::xml::SCOPE)
		{
			scope = std::stoi(_value);
		}
		else if(_name == "contents")
		{ 
			auto contents = utility::tokenize_string(_value, ",");
			for (auto&& c : contents)
			{
				if (c == "vertex")
				{
					record_vertex = true;
				}
				else if (c == "color")
				{
					record_color = true;
				}
				else if (c == "texture")
				{
					record_texture = true;
				}
			}
		}
		else if (_name == "float_list")
		{
			auto s_vec = utility::tokenize_string(_value, ",");
			for (auto&& s : s_vec)
			{
				verts.push_back(std::stof(s));
			}
		}
		else if (_name == "index_list")
		{
			auto i_vec = utility::tokenize_string(_value, ",");
			for (auto&& i : i_vec)
			{
				indicies.push_back(std::stoi(i));
			}
		}

		if (record_vertex)
		{
			if(_name == "vertex_stride")
			{ 
				vertex_stride = std::stoi(_value);
			}
			else if(_name == "vertex_offset")
			{ 
				vertex_offset = std::stoi(_value);
			}
		}

		if (record_texture)
		{
			if(_name == "texture_stride")
			{
				texture_stride = std::stoi(_value);
			}
			else if(_name == "texture_offset")
			{ 
				texture_offset = std::stoi(_value);
			}
		}

		if (record_color)
		{
			if(_name == "color_stride")
			{
				color_stride = std::stoi(_value);
			}
			else if(_name == "color_offset")
			{
				color_offset = std::stoi(_value);
			}
		}
	}

	PLOGV << witchcraft::log_strings::resource_manager_meta_load << "VertexResource";

	// note: we're going to make something derived from EngineResourceBase
	std::unique_ptr<EngineResourceBase> resource;
	resource = std::make_unique<VertexResource>(
		  name				// 
		, filepath			// 
		, type				// 
		, scope				// 
		, verts				//
		, indicies			//
		, vertex_stride		// 
		, vertex_offset		// 
		, color_stride		//
		, color_offset		//
		, texture_stride	// 
		, texture_offset	// 
		);

	return std::move(resource);
}

std::unique_ptr<EngineResourceBase> ResourceManager::build_shader_resource_from_xml(XML::xml_node<> const & xml)
{
	std::string resource_name = "";
	unsigned int scope = 0;

	std::map<std::string, std::string> shader_files;

	for (XML::xml_attribute<> * attr = xml.first_attribute(); attr; attr = attr->next_attribute())
	{
		std::string _name = attr->name();
		std::string _value = attr->value();

		if (_name == witchcraft::xml::NAME)
		{
			resource_name = _value;
		}
		else if (_name == witchcraft::xml::SCOPE)
		{
			scope = std::stoi(_value);
		}
	}
	
	auto resource = std::make_unique<ShaderResource>(resource_name, scope);

	for (XML::xml_node<>* child = xml.first_node(); child; child = child->next_sibling())
	{
		std::string type = "";
		std::string path = "";

		for (XML::xml_attribute<> * attr = child->first_attribute(); attr; attr = attr->next_attribute())
		{
			std::string _name = attr->name();
			std::string _value = attr->value();

			if (_name == witchcraft::xml::TYPE)
			{
				type = _value;
			}
			else if (_name == witchcraft::xml::FILEPATH)
			{
				path = _value;
			}
		}

		resource->shader_files[type] = path;
	}

	std::unique_ptr<EngineResourceBase> shader_resource = std::move(resource);
	return std::move(shader_resource);
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

	for (XML::xml_attribute<> * attr = xml.first_attribute(); attr; attr = attr->next_attribute())
	{
		std::string _name = attr->name();
		std::string _value = attr->value();

		if (_name == witchcraft::xml::NAME)
		{
			animation_name = _value;
		}
		else if (_name == witchcraft::xml::animation_2d_ms_per_frame)
		{
			animation_ms_per_frame = std::stoi(_value);
		}
		else if (_name == witchcraft::xml::animation_2d_sequence)
		{
			auto sv = utility::tokenize_string(_value, witchcraft::xml::delimiter);
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
EngineResourceBase * ResourceManager::find_resource(unsigned int id, int scope)
{
	if (resource_count == 0){ return nullptr; }

	// iterate through all of the scene ids
	for (auto&& resource : resource_map[scope])
	{
		if (resource->id == id)
		{
			return resource.get();
		}
	}
	
	// we went through all existing resource without finding a match
	return nullptr;
}

EngineResourceBase * ResourceManager::find_resource(char const * name, int scope)
{
	if (name == nullptr) { return nullptr; }

	for (auto&& resource : resource_map[scope])
	{
		if (resource->name.find(name) != std::string::npos)
		{
			return resource.get();
		}
	}

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

EngineResourceBase * ResourceManager::load_from_xml_file(std::string const & file)
{
	if (false == utility::file_exists(file))
	{
		PLOGE << "ERROR: FILE DOES NOT EXIST; path: \"" << file << "\"";
		return nullptr;
	}

	XML::file<> config_file(file.c_str());
	XML::xml_document<> doc;
	doc.parse<0>(config_file.data());

	// Top node is also called the "resource tree"
	XML::xml_node<> * top_node = doc.first_node(witchcraft::xml::resource_list);

	if (top_node)
	{
		// enumerate objects
		for (XML::xml_node<> * child = top_node->first_node(); child; child = child->next_sibling())
		{
			std::unique_ptr<EngineResourceBase> resource = nullptr;

			// for each object, enumerate the attributes it contains
			for (XML::xml_attribute<> * attr = child->first_attribute(); attr; attr = attr->next_attribute())
			{
				std::string _name = attr->name();
				std::string _value = attr->value();

				// check resource type
				if (_name == witchcraft::xml::TYPE)
				{
					// We will allow/force resource managers to implement their own derived
					// versions of EngineResourceBase.  Those managers will create the resource,
					// and then give us a unique_ptr<EngineResourceBase> pointer back, and this 
					// scope will need to add the EngineResourceBase pointer to the resource list.
					if (_value == "image")
					{
						resource = build_render_resource_from_xml(*child);
						break;
					}
					else if (_value == "vertex_list")
					{
						resource = build_vertex_resource_from_xml(*child);
						break;
					}
					else if (_value == "shader")
					{
						resource = build_shader_resource_from_xml(*child);
						break;
					}
					else if (_value == "audio")
					{
						// resource = _audio_manager->load_resource_from_xml(child);
						// break;
					}
					else if (_value == "dialogue")
					{
						// resource = build_dialogue_resource_from_xml(*child);
						// break;
					}
					else if (_value == "text")
					{
						// resource = _config_manager->load_resource_from_xml(child);
						// break;
					}
				}
			}

			if (resource)
			{	
				// do not add duplicates of the same file
				if (find_resource(resource->id, resource->scope)) {
					return resource.get(); }

				auto scope = resource->scope;
				// we must use std::move to change ownership of the unique_ptr
				resource_map[scope].push_back(std::move(resource));
				resource_count++;
				return resource_map[scope].back().get();
			}
		}
	}
	
	return nullptr;
}

// WARN: Must be called for each scene change
bool ResourceManager::set_current_scope(unsigned int Scope)
{
	// You cannot change scope, until a global resource is loaded
	if (resource_count == 0) { return false; }
	
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

ResourceManager::ResourceManager(MessageBus * mb)
	: resource_count(0)
	, current_scope(witchcraft::configuration::global_resource_scope)
	, message_bus(mb)
{
	std::function<void(Message)> cb = std::bind(&ResourceManager::handle_message, this, std::placeholders::_1);
	mb->subscribe("resource", cb);
	resource_channel_id = mb->channel_lookup("resource");
	render_channel_id = mb->channel_lookup("render");
	engine_channel_id = mb->channel_lookup("engine");
}

void ResourceManager::handle_message(Message m)
{
	// request resource by name, supplied as <char*>
	if (m.type == MessageType::REQUEST__RESOURCE)
	{
		if (m.data == nullptr){return;}

		auto cptr = static_cast<char*>(m.data);
		auto rptr = this->find_resource(cptr, 0);

		if (rptr != nullptr)
		{
			Message reply
			{
				  m.sender
				, m.recipient
				, MessageType::SUPPLY__RESOURCE
				, rptr
			};
			message_bus->send_direct_message(reply);
		}
	}
	else if (m.type == MessageType::REQUEST__LOAD_RESOURCE)
	{
		if (m.data == nullptr) 
		{ 
			PLOGE << "ERROR! Cannot load resource, no path given!";
			message_bus->log_message(m);
			return; 
		}
		auto cptr = static_cast<char*>(m.data);

		if (cptr == nullptr)
		{
			PLOGE << "ERROR! Cannot load resource!  Data ptr not castable to char *";
			message_bus->log_message(m);
			return;
		}

		auto resource = load_from_xml_file(cptr);
		Message reply = {
			  m.sender
			, m.recipient
			, SUPPLY__RESOURCE
			, (void*) resource
		};
		message_bus->send_direct_message(reply);
	}
}

int ResourceManager::get_current_scope() const
{
	return current_scope;
}

unsigned int ResourceManager::get_resource_count() const
{
	return resource_count;
}
