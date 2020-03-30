#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

// Common includes
#include "plog/Log.h"
#include "../uninitialized_values.h"

class cEngineObject
{
private:
protected:
public:
	unsigned int id;

	cEngineObject()
		: id(uninit::UINT)
	{}

	cEngineObject(unsigned int const & ID)
		: id(ID)
	{}
};

#endif // !ENGINE_OBJECT_H
