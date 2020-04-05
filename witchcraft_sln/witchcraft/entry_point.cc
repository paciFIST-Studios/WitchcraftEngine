// NOTE: MUST be defined before initializing SDL, if you're using your own main()
// we do it before anything else in the program, to make sure it's "not possible"
// to init SDL before this is defined
#define SDL_MAIN_HANDLED

#include "unit_testing_switch.h"
#ifdef RUN_UNIT_TESTS
	#define CATCH_CONFIG_RUNNER
	#include "catch.hpp"
#endif // RUN_UNIT_TESTS

// our logging lib
#include <plog/Log.h>

// witchcraft
#include "string_constants.h"

#include "engine/engine.h"


void init_logging()
{
	// we have to initialize logging manually.  This is set to append to any existing file
	plog::init(plog::verbose, witchcraft::file_strings::engine_log_file_name.c_str());
}

void run_unit_tests()
{
	PLOGV << witchcraft::banners::unit_test_banner;
	PLOGV << witchcraft::log_strings::engine_unit_tests_start;
	Catch::Session session;
	int numberOfFailures = session.run();
	PLOGV << witchcraft::log_strings::engine_unit_tests_stop;
	PLOGV << "\n\n\n\n";
}

int main(int argc, char** argv[])
{
	// do something with arguments

	// logging must come next
	init_logging();

	run_unit_tests();

	auto init = EngineInitializer{ 1 };
	auto engine = Engine(init);

	engine.startup();
	engine.run();
	engine.shutdown();


}
