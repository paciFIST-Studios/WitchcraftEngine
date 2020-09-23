#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

// Common includes
#include "plog/Log.h"

#include "engine_id.h"
#include "../uninitialized_values.h"

class qEngineObject
{
private:
protected:
public:
	unsigned int const id;

	qEngineObject()
		: id(witchcraft::engine::get_engine_id())
	{}
};

#endif // !ENGINE_OBJECT_H
