#ifndef MESSAGE_BUS_H
#define MESSAGE_BUS_H

#include <functional>
#include <map>

#include "../engine/engine_object.h"

enum MessageType
{
	  UNINITIALIZED		= 0
	, TESTING			= 1
	, LOAD_RESOURCE		= 2
	, UNLOAD_RESOURCE	= 3


	// -- this is always the last one
	, ENUM_LENGTH
};

struct Message
{
	unsigned int recipient;
	unsigned int sender;
	MessageType type;
	void * data;
};


class MessageBus : public qEngineObject 
{
public:
	typedef std::function<void(Message m)> CallbackType;

private:
protected:

	// ID, vec<recipient's messages>
	std::map <unsigned int, std::vector<Message>> waiting_messages;

	// ID, vec<callbacks>
	std::map<unsigned int, std::vector<CallbackType>> registered_callbacks;

	// name, channel #
	std::map<char const *, unsigned int> name_to_channel_number {
	//	  { "ai"		, 1  }
	//	, { "animation"	, 2  }
	//	, { "audio"		, 3  }
	//	, { "camera"	, 4  }
	//	, { "cinematic"	, 5  }		
	//	, { "collision"	, 6  }
		  { "console"	, 7  }
		, { "debug"		, 8  }
		, { "engine"	, 9  }
	//	, { "input"		, 10 }
	//	, { "log"		, 11 }
	//	, { "physics"	, 12 }
	//	, { "player"	, 13 }
		, { "render"	, 14 }
		, { "resource"	, 15 }
		, { "scene"		, 16 }
	//	, { "ui"		, 17 }
	};

	bool is_same_fn_address(CallbackType const & fn_a, CallbackType const & fn_b) const
	{
		auto a = fn_a.target<void(*)(Message)>();
		auto b = fn_b.target<void(*)(Message)>();
		return (*a == *b);
	}

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

	void subscribe(char const * channel_name, CallbackType cb)
	{
		auto channel_number = name_to_channel_number[channel_name];
		subscribe(channel_number, cb);
	}

	void subscribe(unsigned int channel_num, CallbackType cb)
	{
		if (registered_callbacks.find(channel_num) != registered_callbacks.end())
		{
			registered_callbacks[channel_num].push_back(cb);
		}
		else
		{
			registered_callbacks[channel_num] = std::vector<CallbackType>{ cb };
		}
	}

	void unsubscribe(char const * channel_name, CallbackType cb)
	{		
		auto channel_number = name_to_channel_number[channel_name];
		unsubscribe(channel_number, cb);
	}
	
	void unsubscribe(unsigned int channel_num, CallbackType cb)
	{
		if (registered_callbacks.find(channel_num) == registered_callbacks.end())
		{
			return; // none found
		}

		// see algorithm example near bottom of article
		// https://www.techiedelight.com/remove-elements-vector-inside-loop-cpp/

		auto iter = registered_callbacks[channel_num].begin();
		while (iter != registered_callbacks[channel_num].end())
		{
			if (is_same_fn_address(cb, *iter))
			{
				iter = registered_callbacks[channel_num].erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	// sent messages accumulate in waiting messages
	void send_message(Message m)
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

	// direct messages go straight to the callbacks
	void send_direct_message(Message m)
	{
		auto vec = registered_callbacks[m.recipient];
		for (auto cb : vec)
		{
			cb(m);
		}
	}


	// taking the messages clears waiting messages
	std::vector<Message> take_messages(char const * channel)
	{
		auto channel_number = name_to_channel_number[channel];
		return take_messages(channel_number);
	}

	// taking the messages clears waiting messages
	std::vector<Message> take_messages(unsigned int channel)
	{
		std::vector<Message> result;

		for (auto m : waiting_messages[channel])
		{
			result.push_back(m);
		}
		
		waiting_messages[channel].clear();

		return result;
	}
};


#endif // !MESSAGE_BUS_H
