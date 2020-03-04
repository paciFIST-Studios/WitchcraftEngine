#include "resource_manager.h"

cResource * cResourceManager::findResourceByID(unsigned int UID)
{
	std::map<unsigned int, std::list<cResource*>>::iterator element_itr;

	// search scopes for correct scope
	for (element_itr = m_Resources.begin(); element_itr != m_Resources.end(); element_itr++)
	{
		if (!(*element_itr).second.empty())
		{
			std::list<cResource*>::iterator list_itr;

			// search elements(resources) of this scope
			for (list_itr = (*element_itr).second.begin(); list_itr != (*element_itr).second.end(); list_itr++)
			{
				// check for id match
				if ((*list_itr)->m_ResourceID == UID)
					return (*list_itr);
			}
		}
	}

	return nullptr;
}

void cResourceManager::clearAll()
{
	std::map<unsigned int, std::list<cResource*>>::iterator element_itr;

	// look through scopes
	for (element_itr = m_Resources.begin(); element_itr != m_Resources.end(); element_itr++)
	{
		if (!(*element_itr).second.empty())
		{
			std::list<cResource*>::iterator list_itr;

			// search the resources for this specific scope
			for (list_itr = (*element_itr).second.begin(); list_itr != (*element_itr).second.end(); list_itr++)
			{
				// delete resrouce object
				(*list_itr)->unload();
				SAFE_DELETE(*list_itr);
			}

			(*element_itr).second.clear();
		}
	}

	m_Resources.clear();
}

bool cResourceManager::loadFromXMLFile(std::string Filename)
{
	TiXmlDocument doc(Filename.c_str());

	if (doc.LoadFile())
	{
		// find top node
		TiXmlNode * ResourceTree = doc.FirstChild("resources");

		if (ResourceTree)
		{
			// enumerate objects
			for (TiXmlNode * child = ResourceTree->FirstChild(); child; child = child->NextSibling())
			{
				TiXmlElement * Element = child->ToElement();

				if (Element)
				{
					cResource * Resource = nullptr;

					for (TiXmlAttribute * ElementAttribute = Element->FirstAttribute();	ElementAttribute; ElementAttribute = ElementAttribute->Next())
					{
						// examine resource object
						std::string AttributeName = ElementAttribute->Name();
						std::string AttributeValue = ElementAttribute->Value();

						// check resourece type
						if (AttributeName == "type")
						{
							// We will allow resource managers to implement their own decendant
							// versions of cResource.  Those managers will create the resource,
							// and then give us a cResource pointer back, and this scope will need
							// to add the cResource pointer to the resourece list.

							if (AttributeValue == "graphic")
							{
								// Resourece = g_RenderManager->loadResourceFromXML(Element);
							}
							if (AttributeValue == "audio")
							{
								// Resource = g_AudioManager->loadResourceFromXML(Element);
							}
							if (AttributeValue == "text")
							{
								//Resource = g_ConfigManager->loadResourceFromXML(Element);
							}
						}

						if (Resource)
						{
							if (AttributeName == "UID")
							{
								Resource->m_ResourceID = atoi(AttributeValue.c_str());
							}
							if (AttributeName == "filename")
							{
								Resource->m_FileName = AttributeValue;
							}
							if (AttributeName == "scenescope")
							{
								Resource->m_Scope = atoi(AttributeValue.c_str());
							}
						}							
					}

					if (Resource)
					{
						// resource added here
						m_Resources[Resource->m_Scope].push_back(Resource);
						m_ResourceCount++;
					}
				}
			}

			return true;
		}
	}

	return false;
}


// WARN: Must be called for each scene change
void cResourceManager::setCurrentScope(unsigned int Scope)
{
	// unload old scope, but not global
	if (m_CurrentScope != 0)
	{
		std::list<cResource*>::iterator list_itr;

		for (list_itr = m_Resources[m_CurrentScope].begin();
			list_itr != m_Resources[m_CurrentScope].end(); 
			list_itr++)
		{
			// note: also calling delete here would kill our cResource obj
			// and that would delete the metadata, which we would use to
			// reload this asset, if the scene becomes active again.
			(*list_itr)->unload();
		}
	}

	m_CurrentScope = Scope;

	// load new scope
	std::list<cResource*>::iterator list_itr;

	for (list_itr = m_Resources[m_CurrentScope].begin();
		list_itr != m_Resources[m_CurrentScope].end();
		list_itr++)
	{
		(*list_itr)->load();
	}
}

