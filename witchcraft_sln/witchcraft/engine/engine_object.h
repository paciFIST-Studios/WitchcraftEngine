#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

// Common includes
#include "plog/Log.h"
#include "../uninitialized_values.h"

class qEngineObject
{
private:
protected:
public:
	unsigned int const id;

	qEngineObject()
		: id(uninit::UINT)
	{}

	qEngineObject(unsigned int const & ID)
		: id(ID)
	{}
};

#endif // !ENGINE_OBJECT_H
