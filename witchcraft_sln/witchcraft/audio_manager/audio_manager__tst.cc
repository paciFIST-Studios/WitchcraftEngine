#ifndef AUDIO_MANAGER__TST_CC
#define AUDIO_MANAGER__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../audio_manager/audio_manager.h"
		
		TEST_CASE("AudioManager::ctor  constructs object   w/ default args")
		{
			auto am = &AudioManager();
			REQUIRE(am);
		}
	
		//TEST_CASE("AudioManager::ctor  constructs object   when given message bus ptr")
		//{
		//	auto mb = &MessageBus();
		//	auto am = &AudioManager(mb);
		//
		//	REQUIRE(mb);
		//	REQUIRE(am);
		//}

	#endif // RUN_UNIT_TESTS

#endif // !AUDIO_MANAGER__TST_CC


