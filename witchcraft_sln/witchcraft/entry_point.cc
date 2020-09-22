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
#include "utility/get_latest_log_str.h"

#include "engine/engine.h"


namespace witchcraft
{
	namespace configuration
	{
		// EB - 20200922 
		// we want to allow people to send messages to and from their engine id,
		// so in order to manager that, make sure we leave a blank spot for named
		// message channels.  See how many we're using in message_bus.h
		static unsigned int const engine_id_offset = 100;
		// EB - 20200922 

		static unsigned int const logging_severity = static_cast<unsigned int>(plog::verbose);
		static unsigned int const log_file_max_size_bytes = 100000; // "marketing" 100k
		static unsigned int const log_file_max_logs = 4;
	}
}


void init_logging()
{
	bool preserve_old_logs = false;
	bool delete_old_logs = true;

	bool record_failure_to_move_prior_log = true;

	if (preserve_old_logs)
	{
		std::string start_name = witchcraft::file_strings::engine_log_file_name;
		if (utility::file_exists(start_name))
		{
			std::string end_name = witchcraft::configuration::get_latest_log_str();
			int result = rename(start_name.c_str(), end_name.c_str());

			if (result >= 0)
			{
				record_failure_to_move_prior_log = false;
			}
		}
	}
	else if (delete_old_logs)
	{
		std::string file = witchcraft::file_strings::engine_log_file_name;
		if (utility::file_exists(file))
		{
			remove(file.c_str());
		}
	}
	
	// This is set to append to any existing file
	plog::init(
		  static_cast<plog::Severity>(witchcraft::configuration::logging_severity)
		, witchcraft::file_strings::engine_log_file_name
		, witchcraft::configuration::log_file_max_size_bytes
		, witchcraft::configuration::log_file_max_logs
	);

	if (record_failure_to_move_prior_log && !delete_old_logs)
	{
		PLOGE << "\n\nFAILED TO RENAME OLD LOG.\n\n";
	}
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

static unsigned int get_engine_id() 
{
	static unsigned int id = witchcraft::configuration::engine_id_offset;
	return id++;
}

int main(int argc, char** argv[])
{
	// do something with arguments

	// logging must come next
	init_logging();

	run_unit_tests();


	auto init = EngineInitializer{ get_engine_id() };
	// todo: give this to the engine using args
	init.project_file_path = "K:/_Git/witchcraft_engine/witchcraft_sln/witchcraft/asset/soccer_game/soccer_game.project";
	auto engine = Engine(init);

	engine.startup();
	engine.run();
	engine.shutdown();
}
