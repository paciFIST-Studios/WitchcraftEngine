#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

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
public:
	unsigned int _resource_id;
	unsigned int _scope;
	std::string _file_name;
	RESOURCE_TYPE _type;

	// to overload
	virtual ~cResource() {}
	virtual void load() {}
	virtual void unload() {}

	inline cResource()
	{
		_resource_id = _scope = 0;
		_type = RESOURCE_NULL;
	}
};

#endif // !RESOURCE_H
