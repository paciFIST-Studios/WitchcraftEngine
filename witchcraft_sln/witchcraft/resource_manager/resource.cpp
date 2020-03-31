#include "resource.h"

unsigned int qResource::get_resource_id() const
{
	return _resource_id;
}

unsigned int qResource::get_scope_id() const
{
	return _scope_id;
}

std::string qResource::get_file_name() const
{
	return _file_name;
}

RESOURCE_TYPE qResource::get_resource_type() const
{
	return _type;
}

qResource::qResource()
	// no args
	// initializer list
	: _resource_id(uninit::UCHAR)
	, _scope_id(uninit::UINT)
	, _file_name(std::string(uninit::CSTRING))
	, _type(RESOURCE_TYPE::RESOURCE_NULL)
{}

qResource::qResource(
	// ctor arguments
	  unsigned int ID
	, unsigned int Scope
	, std::string const & FileName
	, RESOURCE_TYPE ResourceType)
	// initializer list
	: _resource_id(ID)
	, _scope_id(Scope)
	, _file_name(FileName)
	, _type(ResourceType)
{}

