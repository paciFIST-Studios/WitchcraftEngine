#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <list>
#include <map>
#include <string>

#include "tinyxml\tinyxml.h"

#include "engine\engine.h"

typedef enum {
	  RESOURCE_NULL = 0
	, RESOURCE_GRAPHIC = 1
	, RESOURCE_MOVIE = 2
	, RESOURCE_AUDIO = 3
	, RESOURCE_TEXT = 4
} RESOURCE_TYPE;


// ----------------------------------------

// The cResource class represents a resource object, which is managed by a resource manager
// to be inhereted from, by other objects
class cResource : public cEngineObject
{
private:
protected:
public:
	unsigned int m_ResourceID;
	unsigned int m_Scope;
	std::string m_FileName;
	RESOURECE_TYPE m_Type;

	// to overload
	virtual ~cResource() {}
	virtual void load() {}
	virtual void unload() {}

	inline cResource()
	{
		m_ResourceID = m_Scope = 0;
		m_Type = RESOURCE_NULL;
	}

};


// ----------------------------------------

// resource manager, to manage the resource objects
class cResourceManager : public cEngineObject
{
private:
protected:

	// current scope
	unsigned int m_CurrentScope;

	// total resources managed
	unsigned int m_ResourceCount;

public:

	// map<scope, resource_list>
	std::map<unsigned int, std::list<cResource*>> m_Resources;

	// find resource by id.  retun null if not found
	cResource* findResourceByID(unsigned int UID);

	// clears all resources and scopes
	void clearAll();

	// loads resources from xml
	bool loadFromXMLFile(std::string Filename);

	// sets the current scope.  Depends on current scene
	void setCurrentScope(unsigned int Scope);

	const unsigned int getResourceCount(){return m_ResourceCount;}
	
	inline cResourceManager() 
	{
		m_CurrentScope = m_ResourceCount = 0;
	}

};





#endif // RESOURCE_MANAGER_H
