#ifndef MESSAGE_BUS__TST_CC
#define MESSAGE_BUS__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "message_bus.h"

		TEST_CASE(" message_bus::ctor    creates generic object")
		{
			auto mbus = MessageBus();
			REQUIRE(mbus.id == uninit::UINT);
		}

		bool test_returns_false_once_then_true_always()
		{
			static bool called_twice = false;

			if (called_twice == false)
			{
				called_twice = true;
				return false;
			}

			return called_twice;
		}

		TEST_CASE("Message::data    void * can be used to hold a fn ptr")
		{
			//https://www.kdab.com/how-to-cast-a-function-pointer-to-a-void/
			auto fp = &test_returns_false_once_then_true_always;
			bool r1;
			REQUIRE_NOTHROW(r1 = fp());
			REQUIRE(r1 == false);

			auto m = Message{
				  0	// message id
				, 1	// recipient
				, 2	// sender
				, MessageType::FunctionCall
				// cast a fn ptr to a void*
				, reinterpret_cast<void *&>(fp) // data
			};

			// cast a void * to a fn ptr
			auto fp2 = reinterpret_cast<bool(*)()>(m.data);
			bool r2;
			REQUIRE_NOTHROW(r2 = fp2());
			REQUIRE(r2 == true);
		}


		struct TestData
		{
			int a;
			int b;
			int c;
			int d;
		};
		TEST_CASE("Message::data    void * can be used to hold object ptr")
		{
			auto test_data = TestData{ 1, 3, 5, 7 };

			auto m = Message{
				  0	// message id
				, 1	// recipient
				, 2 // sender
				, MessageType::Object
				, &test_data 
			};

			// NOTE: This has to be cast as a ptr again,
			auto obj = (TestData*)m.data;

			REQUIRE(test_data.a == obj->a);
			REQUIRE(test_data.b == obj->b);
			REQUIRE(test_data.c == obj->c);
			REQUIRE(test_data.d == obj->d);
		}

	#endif // RUN_UNIT_TESTS

#endif // !MESSAGE_BUS__TST_CC


