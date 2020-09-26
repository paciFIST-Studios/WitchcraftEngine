#ifndef ENGINE__TST_CC
#define ENGINE__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "engine.h"

		class TestEngine : public Engine
		{
		public:
			TestEngine(EngineInitializer ei)
				: Engine(ei)
			{}

			std::string get_project_file_path() { return this->project_file_path; }
		};

		TEST_CASE(" Engine::ctor  contructs correctly   with default arguments")
		{
			REQUIRE_NOTHROW(Engine());

			auto engine = Engine();
			REQUIRE(engine.get_current_state() == EEngineState::CONSTRUCTED);
		}

		TEST_CASE(" Engine::ctor  constructs correctly  with engine initializer")
		{
			// init with initializer should give the correct id
			std::string test_path = "test_path";
			auto init = EngineInitializer{ true, test_path };
			auto engine = TestEngine(init);
			REQUIRE(engine.get_current_state() == EEngineState::CONSTRUCTED);
			REQUIRE(engine.get_project_file_path() == test_path);
		}

		TEST_CASE(" Engine::startup()")
		{
			auto init = EngineInitializer{ true };
			auto engine = Engine(init);

			engine.startup();
			REQUIRE(engine.get_current_state() == EEngineState::STARTUP);
		}

		TEST_CASE(" Engine::run()")
		{
			auto init = EngineInitializer{ true };
			auto engine = Engine(init);

			engine.startup();
			engine.run();
			REQUIRE(engine.get_current_state() == EEngineState::RUNNING);
		}

		TEST_CASE(" Engine::shutdown()")
		{
			auto init = EngineInitializer{ true };
			auto engine = Engine(init);

			engine.startup();
			engine.run();
			engine.shutdown();
			REQUIRE(engine.get_current_state() == EEngineState::SHUTDOWN);
		}

	#endif // RUN_UNIT_TESTS

#endif // !ENGINE__TST_CC
