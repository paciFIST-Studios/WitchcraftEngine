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
	std::map <unsigned int, std::vector<Message>> waiting_messages;

	// ID, vec<callbacks>
	std::map<unsigned int, std::vector<void(*)(Message)>> registered_callbacks;

public:

	MessageBus()
		: waiting_messages({})
		, registered_callbacks({})
	{}

	~MessageBus()
	{}

	void subscribe(unsigned int channel, void(*cb)(Message))
	{
		if (registered_callbacks.find(channel) != registered_callbacks.end())
		{
			registered_callbacks[channel].push_back(cb);
		}
		else
		{
			registered_callbacks[channel] = std::vector<void(*)(Message)>{ cb };
		}
	}

	void unsubscribe(unsigned int channel, void(*cb)(Message))
	{		
		if (registered_callbacks.find(channel) == registered_callbacks.end())
		{
			return; // none found
		}

		// see algorithm example near bottom of article
		// https://www.techiedelight.com/remove-elements-vector-inside-loop-cpp/

		auto iter = registered_callbacks[channel].begin();
		while (iter != registered_callbacks[channel].end())
		{
			if (*iter == cb)
			{
				iter = registered_callbacks[channel].erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	void push_message(Message m)
	{
		if (waiting_messages.find(m.recipient) != waiting_messages.end())
		{
			waiting_messages[m.recipient].push_back(m);
		}
		else // add new key & vector
		{
			waiting_messages[m.recipient] = std::vector<Message>{ m };
		}
	}


	int peek_subscriber_count(unsigned int id)
	{
		if (registered_callbacks.find(id) == registered_callbacks.end())
		{
			return -1;
		}
		else
		{
			return registered_callbacks[id].size();
		}
	}

	int peek_message_count(unsigned int id)
	{
		if (waiting_messages.find(id) != waiting_messages.end())
		{
			return waiting_messages[id].size();
		}
		else
		{
			return -1;
		}
	}

	std::vector<Message> get_messages(unsigned int id)
	{
		std::vector<Message> result;

		for (auto m : waiting_messages[id])
		{
			result.push_back(m);
		}
		
		waiting_messages[id] = std::vector<Message>();

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

		static void get_nothing() {}

	}

}


#endif // !MESSAGE_BUS_H
