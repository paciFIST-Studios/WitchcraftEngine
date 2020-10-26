#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include <assert.h>
#include <cstdint>
#include <stddef.h>

class LinearAllocator
{
private:
	LinearAllocator & operator=(LinearAllocator const &) = delete;

	struct MemoryChunk
	{
		void *			data;
		MemoryChunk *	previous;
		size_t			size;
		size_t			used_memory;
	};

protected:
	void * _memory_start;
	size_t _current_size;
	size_t _current_used_memory;
	size_t _current_allocations;

public:
	// alignment

	LinearAllocator(){}
	LinearAllocator(size_t size, void * start)
		: _memory_start(start)
		, _current_size(size)
		, _current_used_memory(0)
		, _current_allocations(0)
	{}

	~LinearAllocator() 
	{
		ASSERT(_current_allocations == 0 && _current_used_memory == 0);
		_memory_start = nullptr;
		_current_size = 0;
	}


	void * allocate(size_t size, uint8_t alignment)
	{

	}

	void deallocate(void * memory)
	{

	}

}

#endif // !LINEAR_ALLOCATOR_H
