#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

#include "../engine/engine_object.h"


enum class EResourceType
{
	  UNINITIALIZED		
	, IMAGE
	, SPRITE_ATLAS
	, SHADER
	, ANIMATION
	, MOVIE	
	, AUDIO	
	, TEXT		
	, DIALOGUE
};


///	@file	resource.h
///	@brief	EngineResource represents an engine resource, which can be loaded or unloaded
///	@note	all EngineResource objects are owned by ResourceManager
///	@author	Ellie Barrett
///	@data	20200925
class EngineResource : public EngineObjectBase
{
private:
protected:
public:

	std::string   const name;
	std::string   const filepath;
	EResourceType const type;
	int 		  const scope;
	

	// these are working in the fashion of an IResource interface
	virtual ~EngineResource() {}
	virtual void load()  {}
	virtual void unload(){}


	EngineResource()
		: name("uninit")
		, filepath("uninit")
		, type(EResourceType::UNINITIALIZED)
		, scope(-1)
	{}

	EngineResource(
		  std::string const & name
		, std::string const & filepath
		, EResourceType type
		, int scope
	)
		: name(name)
		, filepath(filepath)
		, type(type)
		, scope(scope)
	{}
};

#endif // !RESOURCE_H
