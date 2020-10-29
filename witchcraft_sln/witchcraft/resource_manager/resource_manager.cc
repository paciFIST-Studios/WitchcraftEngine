#include "resource_manager.h"


char * ResourceManager::determine_version(XML::xml_node<> const & xml) const
{
	std::string const doctype(witchcraft::xml::WITCHCRAFT);
	std::string const version(witchcraft::xml::VERSION);

	if (doctype.compare(xml.name()) != 0)
	{
		PLOGE << "ERROR! File format is incorrect.  Not a witchcraft file.";
		return nullptr;
	}

	auto attr = xml.first_attribute(version.c_str());
	if (attr == nullptr)
	{
		PLOGE << "ERROR! No version information provided with file!";
		return nullptr;
	}

	return attr->value();
}

EngineResourceBase * ResourceManager::register_resource(std::unique_ptr<EngineResourceBase> & resource)
{
	auto ptr = find_resource(resource->id, resource->scope);
	if (ptr != nullptr)
	{
		return ptr;
	}

	auto scope = resource->scope;
	resource_map[scope].push_back(std::move(resource));
	resource_count++;
	return resource_map[scope].back().get();
}


EngineResourceBase * ResourceManager::parse_file_version__unknown(XML::xml_node<> const & node)
{
	PLOGE << "Parsing unknown version files is not supported!  Resource will not be loaded";
	return nullptr;
}


EngineResourceBase *  ResourceManager::parse_file_version__010(XML::xml_node<> const & node)
{
	// when using string::compare(str), it returns 0 on exact match
	int constexpr STRCMP_TRUE = 0;

	// We just got the top node, now, we'll parse the child nodes for engine resources
	// we'll return a ptr to the last one parsed
	//std::unique_ptr<EngineResourceBase> resource = nullptr;
	std::string type = node.first_node()->name();

	if (type.compare("image") == STRCMP_TRUE)
	{
		auto resource = build_render_resource_from_xml(*node.first_node());
		return register_resource(resource);
	}
	else if (type.compare("vertex_list") == STRCMP_TRUE)
	{
		auto resource = build_vertex_resource_from_xml(*node.first_node());
		return register_resource(resource);
	}
	else if (type.compare("shader") == STRCMP_TRUE)
	{
		auto resource = build_shader_resource_from_xml(*node.first_node());
		return register_resource(resource);
	}
	else if (type.compare("audio") == STRCMP_TRUE)
	{
		process_xml_audio_block(*node.first_node());
	}
	else if (type.compare("dialogue") == STRCMP_TRUE)
	{
		//auto resource = build_dialogue_resource_from_xml(*node.first_node());
		//return register_resource(resource);
	}
	else if (type.compare("text") == STRCMP_TRUE)
	{
		//auto resource = _config_manager->load_resource_from_xml(*node.first_node());
		//return register_resource(resource);
	}

	
	// ResourceManager owns all of the files, so it will keep the unique_ptrs.
	// we do plan to emit a non-owning, raw-ptr, to the resource we just loaded
	//if (resource)
	//{
	//	// do not add duplicates of the same file
	//	if (find_resource(resource->id, resource->scope)) {
	//		return resource.get();}
	//
	//	auto scope = resource->scope;
	//	resource_map[scope].push_back(std::move(resource));
	//	resource_count++;
	//	return resource_map[scope].back().get();
	//}

	PLOGE << "ERROR! Could not load resource from XML! type: " << type;
	return nullptr;
}

void ResourceManager::process_xml_audio_block(XML::xml_node<> const & audio)
{
	// each child of an audio block is its own resource
	for (XML::xml_node<> * child = audio.first_node(); child; child = child->next_sibling())
	{
		auto sr = build_sound_resource_from_xml(*child);
		register_resource(sr);
	}
}

std::unique_ptr<EngineResourceBase> ResourceManager::build_sound_resource_from_xml(XML::xml_node<> const & node)
{
	int constexpr STRCMP_TRUE = 0;
	auto designation = std::string(node.name());
	
	// each child of an audio block is it's own audio resource
	EResourceType type;
	std::string name = "";
	std::string path = "";
	unsigned    scope = 0;
	
	// type
	if (designation.compare("sfx") == STRCMP_TRUE)
	{
		type = EResourceType::AUDIO_SFX;
	}
	else if (designation.compare("bgm") == STRCMP_TRUE)
	{
		type = EResourceType::AUDIO_BGM;
	}
	
	// name, path, scope
	for (XML::xml_attribute<> * attr = node.first_attribute(); attr; attr = attr->next_attribute())
	{
		std::string _token = attr->name();
		std::string _value = attr->value();

		if (_token == witchcraft::xml::NAME)
		{
			name = _value;
		}
		else if (_token == witchcraft::xml::FILEPATH)
		{
			path = _value;
		}
		else if (_token == witchcraft::xml::SCOPE)
		{
			scope = std::stoi(_value);
		}
	}

	auto ar = std::make_unique<AudioResource>(name, path, type, scope, message_bus);
	std::unique_ptr<EngineResourceBase> resource = std::move(ar);
	return std::move(resource);
}


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
		std::string _token = attr->name();
		std::string _value = attr->value();

		if (_token == witchcraft::xml::NAME)
		{
			// stoi stands for string-to-integer, and is used for
			// parsing a string to an int
			name = _value;
		}
		else if (_token == witchcraft::xml::FILEPATH)
		{
			filepath = _value;
		}
		else if (_token == witchcraft::xml::SCOPE)
		{
			scope = std::stoi(_value);
		}
		else if (_token == witchcraft::xml::sprite_atlas)
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

	PLOGI << witchcraft::log_strings::resource_manager_meta_load << filepath;

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
			//PLOGV << witchcraft::log_strings::resource_manager_meta_load 
			//	<< "{ atlas=\"" << filepath << "\"  anim=\"" << anim.name << "\" }";
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

	int vertex_stride  = 0;
	int vertex_offset  = 0;
	int texture_stride = 0;
	int texture_offset = 0;
	int color_stride   = 0;
	int color_offset   = 0;

	for (XML::xml_attribute<> * attr = xml.first_attribute(); attr; attr = attr->next_attribute())
	{
		std::string _token = attr->name();
		std::string _value = attr->value();

		if (_token == witchcraft::xml::NAME)
		{
			name = _value;
		}
		else if (_token == witchcraft::xml::SCOPE)
		{
			scope = std::stoi(_value);
		}
		else if (_token == "float_list")
		{
			auto s_vec = utility::tokenize_string(_value, ",");
			for (auto&& s : s_vec)
			{
				verts.push_back(std::stof(s));
			}
		}
		else if (_token == "index_list")
		{
			auto i_vec = utility::tokenize_string(_value, ",");
			for (auto&& i : i_vec)
			{
				indicies.push_back(std::stoi(i));
			}
		}
		else if(_token == "vertex_stride")
		{ 
			vertex_stride = std::stoi(_value);
		}
		else if(_token == "vertex_offset")
		{ 
			vertex_offset = std::stoi(_value);
		}
		else if(_token == "texture_stride")
		{
			texture_stride = std::stoi(_value);
		}
		else if(_token == "texture_offset")
		{ 
			texture_offset = std::stoi(_value);
		}
		else if(_token == "color_stride")
		{
			color_stride = std::stoi(_value);
		}
		else if(_token == "color_offset")
		{
			color_offset = std::stoi(_value);
		}
	}

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

	for (XML::xml_attribute<> * attr = xml.first_attribute(); attr; attr = attr->next_attribute())
	{
		std::string _token = attr->name();
		std::string _value = attr->value();

		if (_token == witchcraft::xml::NAME)
		{
			resource_name = _value;
		}
		else if (_token == witchcraft::xml::SCOPE)
		{
			scope = std::stoi(_value);
		}
	}
	
	auto resource = std::make_unique<ShaderResource>(resource_name, scope);

	for (XML::xml_node<>* child = xml.first_node(); child; child = child->next_sibling())
	{
		std::string type = child->name();
		std::string path = "";

		for (XML::xml_attribute<> * attr = child->first_attribute(); attr; attr = attr->next_attribute())
		{
			std::string _token = attr->name();
			std::string _value = attr->value();

			if (_token == witchcraft::xml::FILEPATH)
			{
				path = _value;
			}
		}

		if (path == "")
		{
			PLOGE << "ERROR! Cannot deserialize file! {"
				<< "\n\tresource name: " << resource_name
				<< "\n\tscope: "  << scope
				<< "\n\ttype: \"" << type << "\""
				<< "\n\tpath: \"" << path << "\""
				<< "\n};"
				;
			// NOTE: returning here to avoid keep resource
			return nullptr;
		}
		else
		{
			if (!utility::file_exists(path))
			{
				PLOGE << "ERROR! File does not exist! path: \"" << path << "\"";
				return nullptr;
			}

			// we need to store the contents of the file, not the path
			std::ifstream infile(path, std::ios::in);
			if (infile.is_open())
			{
				resource->shader_files[type] = std::string();
				resource->shader_files[type].assign(
					  std::istreambuf_iterator<char>(infile)
					, std::istreambuf_iterator<char>()
				);
				infile.close();
			}
		}
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
		std::string _token = attr->name();
		std::string _value = attr->value();

		if (_token == witchcraft::xml::NAME)
		{
			animation_name = _value;
		}
		else if (_token == witchcraft::xml::animation_2d_ms_per_frame)
		{
			animation_ms_per_frame = std::stoi(_value);
		}
		else if (_token == witchcraft::xml::animation_2d_sequence)
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
	XML::xml_node<> * top_node = doc.first_node(witchcraft::xml::WITCHCRAFT);

	if (top_node == nullptr)
	{
		PLOGE << "ERROR! Not a witchcraft formatted file!; path: " << file;
		return nullptr;
	}

	// ver 0.1  == int 10
	// ver 1.21 == int 121
	// ver 3.14 == int 314
	char * ver = determine_version(*top_node);
	int version = (ver) ? int(std::stof(ver) * 100) : 0;

	if (version == 0)
	{
		PLOGE << "ERROR! No version information given for witchcraft file! path: " << file;
		return parse_file_version__unknown(*top_node);
	}
	else if (version == 10)
	{
		return parse_file_version__010(*top_node);
	}
	// else if (version == 11)
	// etc

	PLOGE << "UNKNOWN ERROR!  Error while parsing file: " << file;
	return nullptr;
}


// returns a NON-OWNING ptr
EngineResourceBase * ResourceManager::find_resource(unsigned id, int scope)
{
	if (resource_count == 0)
	{ 
		//PLOGE << "ERROR! Resource was requested, but no resources are being tracked!";
		return nullptr; 
	}

	// check given scope
	for (auto&& resource : resource_map[scope]){
		if (resource->id == id) {
			return resource.get();}
	}

	// check global scope if we didn't already
	if (scope != 0) {
		for (auto&& global : resource_map[0]) {
			if (global->id == id) {
				return global.get();
			}
		}
	}

	// finally, check every other scope, except for [scope] and global
	for (auto&& kvp : resource_map)
	{
		unsigned int key = kvp.first;
		if (key == 0 || key == scope) { continue; }

		for (auto&& known : resource_map[key]) {
			if (known->id == id) {
				return known.get();
			}
		}
	}

	//PLOGE << "WARNING! Searched for unknown resource! id: \"" << id << "\"  scope: " << scope  
	//	<< "\n\tSearching for an unknown resource can be VERY time consuming.  Please fix this now.";
	return nullptr;
}

// returns a NON-OWNING ptr
EngineResourceBase * ResourceManager::find_resource(char const * name, int scope)
{
	if (name == nullptr) 
	{ 
		PLOGE << "ERROR! Resource was requested, but no name found!";
		return nullptr; 
	}

	if (resource_count == 0)
	{
		PLOGE << "ERROR! Resource was requested, but no resources are being tracked!";
		return nullptr;
	}

	// search given scope
	for (auto&& resource : resource_map[scope]){
		if (resource->name.compare(name) == 0){
			return resource.get();}
	}

	// check global scope if we didn't already
	if (scope != 0){
		for (auto&& global : resource_map[0]) {
			if (global->name.compare(name) == 0) {
				return global.get();
			}
		}
	}

	// finally, check every other scope, except for [scope] and global
	for (auto&& kvp : resource_map)
	{
		unsigned int key = kvp.first;
		if (key == 0 || key == scope) { continue; }

		for (auto&& known : resource_map[key]){
			if (known->name.compare(name) == 0){
				return known.get();}
		}
	}
	
	//PLOGE << "WARNING! Searched for unknown resource! name: \"" << name << "\"  scope: " << scope
	//	  << "\n\tSearching for an unknown resource can be VERY time consuming.  Please fix this now.";
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
	render_channel_id   = mb->channel_lookup("render"  );
	engine_channel_id   = mb->channel_lookup("engine"  );
}

void ResourceManager::handle_message(Message m)
{
	// request resource by name, supplied as <char*>
	if (m.type == MessageType::REQUEST__RESOURCE)
	{
		if (m.data == nullptr)
		{
			PLOGE << "ERROR! REQUEST__RESOURCE message was sent, but no resource was specified!";
			message_bus->log_message(m);
			return;
		}

		auto cptr = static_cast<char const *>(m.data);
		auto rptr = this->find_resource(cptr, current_scope);

		if (rptr == nullptr)
		{
			PLOGE << "ERROR!  Could not find resource!";
			message_bus->log_message(m);
			return;
		}

		Message reply
		{
			  m.sender
			, m.recipient
			, MessageType::SUPPLY__RESOURCE
			, rptr
		};
		message_bus->send_direct_message(reply);
	}
	else if (m.type == MessageType::REQUEST__LOAD_RESOURCE)
	{
		if (m.data == nullptr) 
		{ 
			PLOGE << "ERROR! Cannot load resource, no path given!";
			message_bus->log_message(m);
			return; 
		}
		auto cptr = static_cast<char const *>(m.data);

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
