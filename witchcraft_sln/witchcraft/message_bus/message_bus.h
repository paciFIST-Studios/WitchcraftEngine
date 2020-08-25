#ifndef MESSAGE_BUS_H
#define MESSAGE_BUS_H

#include <functional>
#include <map>

#include "../engine/engine_object.h"
//#include "../message_bus/ICanSendMessages.h"
//#include "../message_bus/ICanReceiveMessages.h"

enum MessageType
{
	  UNINITIALIZED = 0
	  // used for testing, does not signify specific decoding
	, TESTING
};

struct Message
{
public:
	unsigned int message_id;
	unsigned int recipient;
	unsigned int sender;
	MessageType type;
	void * data;
};

class MessageBus 
	: public qEngineObject 
	//, public ICanSendMessages
	//, public ICanReceiveMessages
{

private:
protected:

	// ID, vec<recipient's messages>
	std::map <unsigned int, std::vector<Message>> message_map;

	virtual void onMessage(Message m){}

public:

	MessageBus()
		: message_map({})
	{}

	~MessageBus()
	{}

	// subscribe
	// un-subscribe

	void push_message(Message m)
	{
		bool result = false;
		if (message_map.find(m.recipient) != message_map.end())
		{
			message_map[m.recipient].push_back(m);
		}
		else // add new key & vector
		{
			message_map[m.recipient] = std::vector<Message>{ m };
		}
	}

	//bool push_interrupt_message(Message m);
	// send interrupting message
	// send delayed message

	int peek_message_count(unsigned int id)
	{
		if (message_map.find(id) != message_map.end())
		{
			return message_map[id].size();
		}
		else
		{
			return -1;
		}
	}

	std::vector<Message> get_messages(unsigned int id)
	{
		std::vector<Message> result;

		for (auto m : message_map[id])
		{
			result.push_back(m);
		}
		
		message_map[id] = std::vector<Message>();

		return result;
	}

};


namespace witchcraft
{
	namespace testing
	{
		struct FourIntStruct
		{
			int a = 0;
			int b = 0;
			int c = 0;
			int d = 0;
		};

		static unsigned int get_times_called()
		{
			static unsigned int count = 0;
			return ++count;
		}

	}

}


#endif // !MESSAGE_BUS_H
