#ifndef MESSAGE_BUS__TST_CC
#define MESSAGE_BUS__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "message_bus.h"

		TEST_CASE("Message::data    void * can be used to hold a fn ptr")
		{
			//https://www.kdab.com/how-to-cast-a-function-pointer-to-a-void/
			auto fp = &witchcraft::testing::get_times_called;
			auto a = fp();
			auto b = fp();
			REQUIRE_NOTHROW(fp());
			REQUIRE(a < b);

			auto m = Message{
				  0	// message id
				, 1	// recipient
				, 2	// sender
				, MessageType::TESTING
				// cast a fn ptr to a void*
				, reinterpret_cast<void *&>(fp) // data
			};

			if (m.type == MessageType::TESTING) 
			{
				// cast a void * to a fn ptr
				auto fp2 = reinterpret_cast<unsigned int(*)(void)>(m.data);
				auto c = fp2();
				auto d = fp2();

				REQUIRE_NOTHROW(fp2());
				REQUIRE(c < d);
				REQUIRE(a < c);
				REQUIRE(b < d);
			}
		}



		TEST_CASE("Message::data    void * can be used to hold object ptr")
		{
			using witchcraft::testing::FourIntStruct;
			auto test_data = FourIntStruct{ 1, 3, 5, 7 };

			auto m = Message{
				  0	// message id
				, 1	// recipient
				, 2 // sender
				, MessageType::TESTING
				, &test_data 
			};

			if (m.type == MessageType::TESTING)
			{
				// NOTE: This has to be cast as a ptr again,
				auto obj = (FourIntStruct*)m.data;

				REQUIRE(test_data.a == obj->a);
				REQUIRE(test_data.b == obj->b);
				REQUIRE(test_data.c == obj->c);
				REQUIRE(test_data.d == obj->d);
			}
		}


		TEST_CASE(" MessageBus::ctor    creates generic object")
		{
			auto mbus = MessageBus();
			REQUIRE(mbus.id == uninit::UINT);
		}


		TEST_CASE(" MessageBus::push_message    pushes message onto vector   when given correct args")
		{
			auto data = witchcraft::testing::FourIntStruct{ 7, 7, 7, 7 };

			int const RECIPIENT_ID = 666;

			auto mb = MessageBus();
			auto m = Message{
				  1	// message id
				, RECIPIENT_ID
				, 3	// sender
				, MessageType::TESTING
				, &data
			};

			REQUIRE(mb.peek_message_count(RECIPIENT_ID) == -1);
			mb.push_message(m);
			REQUIRE(mb.peek_message_count(RECIPIENT_ID) == 1);
			mb.push_message(m);
			REQUIRE(mb.peek_message_count(RECIPIENT_ID) == 2);
			mb.push_message(m);
			REQUIRE(mb.peek_message_count(RECIPIENT_ID) == 3);

		}

	#endif // RUN_UNIT_TESTS

#endif // !MESSAGE_BUS__TST_CC


