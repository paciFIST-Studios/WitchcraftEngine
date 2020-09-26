#include "resource.h"

unsigned int qResource::get_resource_id() const
{
	return _uuid;
}

unsigned int qResource::get_scope_id() const
{
	return _scope;
}

std::string qResource::get_file_name() const
{
	return _file_name;
}

EResourceType qResource::get_resource_type() const
{
	return _type;
}

qResource::qResource()
	// no args
	// initializer list
	: _uuid(uninit::UCHAR)
	, _scope(uninit::UINT)
	, _file_name(std::string(uninit::CSTRING))
	, _type(EResourceType::RESOURCE_NULL)
{}

qResource::qResource(
	// ctor arguments
	  unsigned int ID
	, unsigned int Scope
	, std::string const & FileName
	, EResourceType ResourceType)
	// initializer list
	: _uuid(ID)
	, _scope(Scope)
	, _file_name(FileName)
	, _type(ResourceType)
{}

