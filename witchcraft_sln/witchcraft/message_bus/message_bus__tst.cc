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
				  1	// recipient
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
				  1	// recipient
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
				  RECIPIENT_ID
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

		TEST_CASE(" MessageBus    keeps track of several different recipients")
		{
			auto m = Message{ 1, 0, MessageType::TESTING, nullptr };
			auto mb = MessageBus();

			m.recipient = 1;
			mb.push_message(m);

			m.recipient = 2;
			mb.push_message(m);
			mb.push_message(m);

			m.recipient = 3;
			mb.push_message(m);
			mb.push_message(m);
			mb.push_message(m);

			REQUIRE(mb.peek_message_count(1) == 1);
			REQUIRE(mb.peek_message_count(2) == 2);
			REQUIRE(mb.peek_message_count(3) == 3);
		}


		void mock_OnMessage(Message m){}
		TEST_CASE(" MessageBus::subscribe    allows objects to subscribe to updates    by supplying callback ptr")
		{
			// note: this is explicitly allowing duplicates right now
			auto mb = MessageBus();

			// uninit
			REQUIRE(mb.peek_subscriber_count(1) == -1);

			// +, -, 0
			mb.subscribe(1, mock_OnMessage);
			mb.unsubscribe(1, mock_OnMessage);
			REQUIRE(mb.peek_subscriber_count(1) == 0);

			mb.subscribe(1, mock_OnMessage);
			REQUIRE(mb.peek_subscriber_count(1) == 1);
			mb.subscribe(1, mock_OnMessage);
			REQUIRE(mb.peek_subscriber_count(1) == 2);
			mb.subscribe(1, mock_OnMessage);
			REQUIRE(mb.peek_subscriber_count(1) == 3);
		}

		void mock_a(Message m) {}
		void mock_b(Message m) {}
		void mock_c(Message m) {}
		TEST_CASE(" MessageBus::subscribe    allows subscription to different ids")
		{
			auto mb = MessageBus();

			int const UNINIT = -1;
			int const ONE = 1;
			int const TWO = 2;
			int const THREE = 3;

			REQUIRE(mb.peek_subscriber_count(ONE) == UNINIT);
			REQUIRE(mb.peek_subscriber_count(TWO) == UNINIT);
			REQUIRE(mb.peek_subscriber_count(THREE) == UNINIT);

			// add to channel one
			mb.subscribe(ONE, mock_a);
			REQUIRE(mb.peek_subscriber_count(ONE) == 1);
			mb.subscribe(ONE, mock_b);
			REQUIRE(mb.peek_subscriber_count(ONE) == 2);

			// other channels remain empty
			REQUIRE(mb.peek_subscriber_count(TWO) == UNINIT);
			REQUIRE(mb.peek_subscriber_count(THREE) == -1);

			// add to channel 2	
			mb.subscribe(TWO, mock_c);
			REQUIRE(mb.peek_subscriber_count(TWO) == 1);

			// three remains empty
			REQUIRE(mb.peek_subscriber_count(THREE) == UNINIT);

			// add to channel 3
			mb.subscribe(THREE, mock_c);
			REQUIRE(mb.peek_subscriber_count(THREE) == 1);
		}


		TEST_CASE(" MessageBus::unsubscribe    allows a callback to be removed from a single channel")
		{
			auto mb = MessageBus();

			int const ONE = 1;
			int const TWO = 2;

			// in this test, mock_a is going to represent our object's onMessage() fn
			mb.subscribe(ONE, mock_a);

			// other people are subscribed to the same channel
			mb.subscribe(ONE, mock_b);
			mb.subscribe(ONE, mock_c);

			// and out object is subscribed to a different channel
			mb.subscribe(TWO, mock_a);

			REQUIRE(mb.peek_subscriber_count(ONE) == 3);

			// Now, we want to remove our object's subscription from channel ONE
			// but we don't want to alter the subscription to channel TWO
			mb.unsubscribe(ONE, mock_a);

			// if you unregister something which doesn't exist, then
			// it should not crash
			REQUIRE_NOTHROW(mb.unsubscribe(ONE, mock_a));

			REQUIRE(mb.peek_subscriber_count(ONE) == 2);
			REQUIRE(mb.peek_subscriber_count(TWO) == 1);
		}

	#endif // RUN_UNIT_TESTS

#endif // !MESSAGE_BUS__TST_CC


