#ifndef SHADER_RESOURCE_H
#define SHADER_RESOURCE_H

#include "../resource_manager/resource.h"

class ShaderResource : public qResource
{
private:
protected:
public:
	// file type, file path
	std::map<std::string, std::string> shader_files;

	ShaderResource(unsigned int uuid, unsigned int scope)
		: qResource(uuid, scope, "", EResourceType::RESOURCE_GRAPHIC)
	{}

	~ShaderResource() override {}
	void load() override 
	{ 
		std::stringstream ss;

		ss << "WARNING: cannot load() a ShaderResouce: {";
		for (auto&& kvp : shader_files)
		{
			ss << "\n\t" << kvp.first << " : " << kvp.second << "";
		}
		ss << "};";
		PLOGW << ss.str();
	}
	
	void unload() override 
	{
		std::stringstream ss;

		ss << "WARNING: cannot unload() a ShaderResouce: {";
		for (auto&& kvp : shader_files)
		{
			ss << "\n\t" << kvp.first << " : " << kvp.second << "";
		}
		ss << "};";
		PLOGW << ss.str();
	}
};

#endif // !SHADER_RESOURCE_H
