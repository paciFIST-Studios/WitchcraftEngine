#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include "plog/Log.h"

#include "engine_id.h"

class EngineObjectBase
{
private:
protected:
public:
	unsigned int const id;

	EngineObjectBase()
		: id(witchcraft::engine::get_engine_id())
	{}
};

#endif // !ENGINE_OBJECT_H
