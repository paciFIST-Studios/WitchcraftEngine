#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

#include "plog/Log.h"

#include "../uninitialized_values.h"
#include "../engine/engine_object.h"

#define SAFE_DELETE(a) { delete(a); (a)=nullptr; }

enum class EResourceType : unsigned char
{
	  RESOURCE_NULL			= 0x01
	, RESOURCE_GRAPHIC		= 0x02
	, RESOURCE_ANIMATION	= 0x04
	, RESOURCE_MOVIE		= 0x08
	, RESOURCE_AUDIO		= 0x10
	, RESOURCE_TEXT			= 0x20
	// 0x40
	// 0x80
};

// The qResource class represents a resource object, which is managed by a resource manager
// to be inhereted from, by other objects
class qResource : public qEngineObject
{
private:
protected:
	unsigned int _uuid;
	unsigned int _scope;
	std::string _file_name;
	EResourceType _type;

public:
	
	unsigned int get_resource_id() const;
	
	unsigned int get_scope_id() const;

	std::string get_file_name() const;

	EResourceType get_resource_type() const;

	// these are working in the fashion of an IResource interface
	virtual ~qResource()
	{ 
		PLOGV 
			<< "resource id: " << _uuid 
			<< "  file: " << _file_name 
			<< "  qResource::~qResource()";
	}
	virtual void load()   
	{ 
		PLOGV 
			<< "resource id: " << _uuid 
			<< "  file: " << _file_name 
			<< "  qResource::load()";
	}
	virtual void unload() 
	{ 
		PLOGV
			<< "resource id: " << _uuid
			<< "  file: " << _file_name
			<< "  qResource::unload()";
	}
	   	 
	qResource();

	qResource(unsigned int ID, unsigned int Scope, std::string const & FileName
		, EResourceType ResourceType);

};

#endif // !RESOURCE_H
