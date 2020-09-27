#ifndef ENGINE_RESOURCE_BASE_H
#define ENGINE_RESOURCE_BASE_H

#include <string>

#include "../engine/engine_object.h"


enum class EResourceType
{
	  UNINITIALIZED		
	, IMAGE
	, VERTEX_LIST
	, SPRITE_ATLAS
	, SHADER
	, ANIMATION
	, MOVIE	
	, AUDIO	
	, TEXT		
	, DIALOGUE
};


///	@file	resource.h
///	@brief	EngineResourceBase represents an engine resource, which can be loaded or unloaded
///	@note	all EngineResourceBase objects are owned by ResourceManager
///	@author	Ellie Barrett
///	@data	20200925
class EngineResourceBase : public EngineObjectBase
{
private:
protected:
public:

	std::string   const name;
	std::string   const filepath;
	EResourceType const type;
	int 		  const scope;
	

	// these are working in the fashion of an IResource interface
	virtual ~EngineResourceBase() {}
	virtual void load()  {}
	virtual void unload(){}


	EngineResourceBase()
		: name("uninit")
		, filepath("uninit")
		, type(EResourceType::UNINITIALIZED)
		, scope(-1)
	{}

	EngineResourceBase(
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

#endif // !ENGINE_RESOURCE_BASE_H
