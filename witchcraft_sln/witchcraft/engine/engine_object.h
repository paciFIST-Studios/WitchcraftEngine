#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include "plog/Log.h"

#include "engine_id.h"
#include "../memory/allocation_tracker.h"

class EngineObjectBase
{
private:
protected:
public:
	unsigned int const id;

	EngineObjectBase()
		: id(witchcraft::engine::get_engine_id())
	{}
	   
	/// override of new operator which writes allocations to log
	void * operator new (size_t size)
	{
		PLOGV << "[memory] Alloc: " << size << " bytes";
		witchcraft::memory::allocation_tracker.total_allocated += size;
		return malloc(size);
	}

	/// override of delete operator which writes deletes to log
	void operator delete(void * memory, size_t size)
	{
		PLOGV << "[memory] Free: " << size << " bytes";
		witchcraft::memory::allocation_tracker.total_freed += size;
		free(memory);
	}
};

#endif // !ENGINE_OBJECT_H
