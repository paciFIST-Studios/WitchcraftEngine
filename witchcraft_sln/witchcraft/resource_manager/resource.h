#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

#include "plog/Log.h"

#include "../uninitialized_values.h"
#include "../engine/engine_object.h"

#define SAFE_DELETE(a) { delete(a); (a)=nullptr; }

typedef enum {
	  RESOURCE_NULL			= 0
	, RESOURCE_GRAPHIC		= 1
	, RESOURCE_ANIMATION	= 2
	, RESOURCE_MOVIE		= 3
	, RESOURCE_AUDIO		= 4
	, RESOURCE_TEXT			= 5
} RESOURCE_TYPE;

// The cResource class represents a resource object, which is managed by a resource manager
// to be inhereted from, by other objects
class cResource : public cEngineObject
{
private:
protected:
	unsigned int _resource_id;
	unsigned int _scope_id;
	std::string _file_name;
	RESOURCE_TYPE _type;

public:
	
	unsigned int get_resource_id() const;
	
	unsigned int get_scope_id() const;

	std::string get_file_name() const;

	RESOURCE_TYPE get_resource_type() const;

	// these are working in the fashion of an IResource interface
	virtual ~cResource()
	{ 
		PLOGV 
			<< "resource id: " << _resource_id 
			<< "  file: " << _file_name 
			<< "  cResource::~cResource()";
	}
	virtual void load()   
	{ 
		PLOGV 
			<< "resource id: " << _resource_id 
			<< "  file: " << _file_name 
			<< "  cResource::load()";
	}
	virtual void unload() 
	{ 
		PLOGV
			<< "resource id: " << _resource_id
			<< "  file: " << _file_name
			<< "  cResource::unload()";
	}
	   	 
	cResource();

	cResource(unsigned int ID, unsigned int Scope, std::string const & FileName
		, RESOURCE_TYPE ResourceType);

};

#endif // !RESOURCE_H
