#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

#include "plog/Log.h"

#include "../engine/engine.h"

#define SAFE_DELETE(a) { delete(a); (a)=nullptr; }

typedef enum {
	RESOURCE_NULL = 0
	, RESOURCE_GRAPHIC = 1
	, RESOURCE_MOVIE = 2
	, RESOURCE_AUDIO = 3
	, RESOURCE_TEXT = 4
} RESOURCE_TYPE;

// The cResource class represents a resource object, which is managed by a resource manager
// to be inhereted from, by other objects
class cResource : public cEngineObject
{
private:
protected:
	unsigned int _resource_id;
	unsigned int _scope;
	std::string _file_name;
	RESOURCE_TYPE _type;

public:

	static unsigned int  const UNINIT_RESOURCE_ID	= 0;
	static unsigned int  const UNINIT_RESOURCE_SCOPE = UINT32_MAX;
	static constexpr char const * UNINIT_FILE_NAME = "UNINITIALIZED FILE NAME";
	static RESOURCE_TYPE const UNINIT_RESOURCE_TYPE  = RESOURCE_TYPE::RESOURCE_NULL;
	
	unsigned int get_resource_id() 
	{ 
		return _resource_id; 
	}

	unsigned int get_scope() 
	{ 
		return _scope; 
	}

	std::string get_file_name() 
	{
		return _file_name; 
	}

	RESOURCE_TYPE get_resource_type() 
	{
		return _type; 
	}

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
	   	 
	cResource()
		// no args
		// initializer list
		: _resource_id(UNINIT_RESOURCE_ID)
		, _scope(UNINIT_RESOURCE_SCOPE)
		, _file_name(std::string(UNINIT_FILE_NAME))
		, _type(UNINIT_RESOURCE_TYPE)
	{}

	cResource(
		// ctor arguments
		  unsigned int ID
		, unsigned int Scope
		, std::string const & FileName
		, RESOURCE_TYPE ResourceType)

		// initializer list
		: _resource_id(ID)
		, _scope(Scope)
		, _file_name(FileName)
		, _type(ResourceType)
	{}
};

#endif // !RESOURCE_H
