#ifndef ALLOCATION_TRACKER_H
#define ALLOCATION_TRACKER_H

#include <cstdint>

namespace witchcraft
{
	namespace memory
	{
		struct AllocationTracker
		{
			uint32_t total_allocated = 0;
			uint32_t total_freed = 0;

			uint32_t current_usage() { return total_allocated - total_freed; }
		};

		static AllocationTracker allocation_tracker;
	}
}

#endif // !ALLOCATION_TRACKER_H
