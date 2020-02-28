#pragma once

// define with any number in order to turn it on
#define RUN_UNIT_TESTS = -666

#ifdef RUN_UNIT_TESTS

	// Common includes for unit tests
	#include <string>
	#include "logging\error_log_manager.h"

#endif // RUN_UNIT_TESTS
