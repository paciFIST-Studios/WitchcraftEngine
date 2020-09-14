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

	// name, channel #
	std::map<char const *, unsigned int> name_to_channel_number {
		  { "ai"		, 1  }
		, { "animation"	, 2  }
		, { "audio"		, 3  }
		, { "camera"	, 4  }
		, { "cinematic"	, 5  }		
		, { "collision"	, 6  }
		, { "console"	, 7  }
		, { "debug"		, 8  }
		, { "engine"	, 9  }
		, { "input"		, 10 }
		, { "log"		, 11 }
		, { "physics"	, 12 }
		, { "player"	, 13 }
		, { "render"	, 14 }
		, { "resource"	, 15 }
		, { "ui"		, 16 }
	};

public:

	MessageBus()
		: waiting_messages({})
		, registered_callbacks({})
	{}

	~MessageBus()
	{}

	unsigned int channel_lookup(char const * channel_name)
	{
		return name_to_channel_number[channel_name];
	}

	void subscribe(char const * channel, void(*cb)(Message))
	{
		auto channel_number = name_to_channel_number[channel];
		subscribe(channel_number, cb);
	}

	void subscribe(unsigned int id, void(*cb)(Message))
	{
		if (registered_callbacks.find(id) != registered_callbacks.end())
		{
			registered_callbacks[id].push_back(cb);
		}
		else
		{
			registered_callbacks[id] = std::vector<void(*)(Message)>{ cb };
		}
	}

	void unsubscribe(char const * channel, void(*cb)(Message))
	{		
		auto channel_number = name_to_channel_number[channel];
		unsubscribe(channel_number, cb);
	}

	void unsubscribe(unsigned int id, void(*cb)(Message))
	{
		if (registered_callbacks.find(id) == registered_callbacks.end())
		{
			return; // none found
		}

		// see algorithm example near bottom of article
		// https://www.techiedelight.com/remove-elements-vector-inside-loop-cpp/

		auto iter = registered_callbacks[id].begin();
		while (iter != registered_callbacks[id].end())
		{
			if (*iter == cb)
			{
				iter = registered_callbacks[id].erase(iter);
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

	int peek_subscriber_count(char const * channel)
	{
		auto channel_number = name_to_channel_number[channel];
		return peek_subscriber_count(channel_number);
	}

	int peek_subscriber_count(unsigned int channel)
	{
		if (registered_callbacks.find(channel) == registered_callbacks.end())
		{
			return -1;
		}
		else
		{
			return registered_callbacks[channel].size();
		}
	}


	int peek_message_count(char const * channel)
	{
		auto channel_number = name_to_channel_number[channel];
		return peek_message_count(channel_number);
	}

	int peek_message_count(unsigned int channel)
	{
		if (waiting_messages.find(channel) != waiting_messages.end())
		{
			return waiting_messages[channel].size();
		}
		else
		{
			return -1;
		}
	}

	std::vector<Message> get_messages(char const * channel)
	{
		auto channel_number = name_to_channel_number[channel];
		return get_messages(channel_number);
	}

	std::vector<Message> get_messages(unsigned int channel)
	{
		std::vector<Message> result;

		for (auto m : waiting_messages[channel])
		{
			result.push_back(m);
		}
		
		waiting_messages[channel] = std::vector<Message>();

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
