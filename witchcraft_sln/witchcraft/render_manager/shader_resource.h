#ifndef SHADER_RESOURCE_H
#define SHADER_RESOURCE_H

#include "../resource_manager/engine_resource_base.h"

/// @file	shader_resource.h
/// @brief	ShaderResource holds strings to shader files, and ifentifies them
/// @note	load/unload do nothing, b/c the shader program is built/owned by RenderManager
/// @date	20200926
class ShaderResource : public EngineResourceBase
{
private:
protected:
public:
	/// @brief	Key=Shader Type, value = filepath
	std::map<std::string, std::string> shader_files;

	/// @note	sets EngineResourceBase::filepath to ""
	ShaderResource(std::string name, int scope )
		: EngineResourceBase(name, "", EResourceType::SHADER, scope)
	{}

	~ShaderResource() override {}
	void load() override {};
	void unload() override {};
};

#endif // !SHADER_RESOURCE_H
