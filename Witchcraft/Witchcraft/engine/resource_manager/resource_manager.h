#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <list>
#include <map>
#include <string>

#include "../../../lib/rapidxml/rapidxml.hpp"
#include "../../../lib/rapidxml/rapidxml_utils.hpp"
#define XML rapidxml


#include "../exception.h"
#include "../engine.h"
#include "../../utility/utility.h"


typedef enum {
	  RESOURCE_NULL = 0
	, RESOURCE_GRAPHIC = 1
	, RESOURCE_MOVIE = 2
	, RESOURCE_AUDIO = 3
	, RESOURCE_TEXT = 4
} RESOURCE_TYPE;

#define GLOBAL_SCOPE 0


// ----------------------------------------

#define SAFE_DELETE(a) { delete(a); (a)=nullptr; }

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
	RESOURCE_TYPE m_Type;

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
	

	bool createConfigFiles()
	{
		std::cout << "createConfigFiles()\n";

		std::list<std::string> config_files = {
			std::string("witchcraft.cfg")
		};

		
		for (auto cfg_itr = config_files.begin(); cfg_itr != config_files.end(); cfg_itr++)
		{
			if (utility::fileExists(*cfg_itr))
			{
				std::cout << "\tskipping file: " << *cfg_itr << "\n";
				continue;
			}
				
			std::ofstream cfg_file;
			cfg_file.open(*cfg_itr);
			cfg_file << "\0";
			cfg_file.close();

			if (utility::fileExists(*cfg_itr))
			{
				std::cout << "\tfile created: " << *cfg_itr << "\n";
			}
		}
		
		return true;
	}

	//bool parseConfigFiles()
	//{
	//	// due to how rapidxml works, we start by loading all of this stuff into memory
	//
	//	std::list<char const *> filenames = {
	//		"config.xml"
	//	};
	//
	//	for (auto file_itr = filenames.begin(); file_itr != filenames.end(); file_itr++)
	//	{
	//		try
	//		{
	//			XML::file<> config_file(*file_itr);
	//		}
	//		catch(cException ex)
	//		{
	//			return false;
	//		}
	//
	//		cResource * resource = nullptr;
	//		resource->m_Scope = GLOBAL_SCOPE;
	//		resource->m_Type = RESOURCE_TYPE::RESOURCE_TEXT;
	//		resource->m_FileName = *file_itr;
	//
	//
	//		m_Resources[GLOBAL_SCOPE];
	//	}
	//
	//	return true;
	//}
	//
	inline cResourceManager() 
	{
		m_CurrentScope = m_ResourceCount = 0;
		
		createConfigFiles();
	}

};





#endif // RESOURCE_MANAGER_H
