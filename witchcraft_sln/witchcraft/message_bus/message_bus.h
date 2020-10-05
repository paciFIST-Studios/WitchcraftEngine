#ifndef MESSAGE_BUS_H
#define MESSAGE_BUS_H

#include <functional>
#include <map>
#include <vector>

#include "../engine/engine_object.h"

enum MessageType
{
	  UNINIT
	, TESTING

	, REQUEST__LOAD_RESOURCE
	, REQUEST__UNLOAD_RESOURCE

	/// requests for the audio manager to load file from the attached path
	, REQUEST__AUDIO_LOAD
	/// requests for the audio manager to unload file from the attached path
	, REQUEST__AUDIO_UNLOAD

	/// requests for audio manager to play file with attached name
	, REQUEST__AUDIO_START
	/// like requrest start, but plays on loop
	, REQUEST__AUDIO_START_LOOP
	/// requests for audio manager to stop file with attached name
	, REQUEST__AUDIO_STOP

	/// requests a resource from the resource manager
	, REQUEST__RESOURCE
	/// resoruce manager returns a non-owning pointer to resource
	, SUPPLY__RESOURCE

	/// Console is given a <char const *>, containing a command
	, INVOKE__CONSOLE_COMMAND
	
	/// remderer is given a <char const *>, containing a command
	, INVOKE__RENDER_COMMAND

	/// requests engine to return a non-owning pointer to the debug console
	, REQUEST__CONSOLE_PTR_NON_OWNER

	/// engine supplies a non-owning pointer to the debug console
	, SUPPLY__CONSOLE_PTR_NON_OWNER

	/// requests message bus to send a ping back w/ {data = nullptr;}
	, REQUEST__PING_ME_BACK

	/// -- this is always the last one
	, ENUM_LENGTH
};

// EB - 20200924
// in order to convert this enum to a string, representing the variable name,
// we either have to do something hella fancy, or we have to maintain this
// map of enums to strings.  I have chosen the map, because I know it will work,
// and I apologize to future engineers (including myself) who have to maintain it
// I don't think we should do this for all enums, but I do think it's a good idea
// for the messaging system, since that's the foundation of all engine communications
static std::map<MessageType, char const *> const message_type_enum_to_string {
	  { MessageType::UNINIT						, "UNINIT"						}
	, { MessageType::TESTING					, "TESTING"						}
	, { MessageType::REQUEST__LOAD_RESOURCE		, "REQUEST__LOAD_RESOURCE"		}
	, { MessageType::REQUEST__UNLOAD_RESOURCE	, "REQUEST__UNLOAD_RESOURCE"	}
	, { MessageType::REQUEST__RESOURCE			, "REQUEST__RESOURCE"			}
	, { MessageType::SUPPLY__RESOURCE			, "SUPPLY__RESOURCE"			}
	, { MessageType::INVOKE__CONSOLE_COMMAND	, "INVOKE__CONSOLE_COMMAND"		}
	, { MessageType::INVOKE__RENDER_COMMAND		, "INVOKE__RENDER_COMMAND"		}
	, { MessageType::REQUEST__CONSOLE_PTR_NON_OWNER , "REQUEST__CONSOLE_PTR_NON_OWNER"}
	, { MessageType::SUPPLY__CONSOLE_PTR_NON_OWNER  , "SUPPLY__CONSOLE_PTR_NON_OWNER" }
	, { MessageType::REQUEST__PING_ME_BACK		, "REQUEST__PING_ME_BACK"		}

	// -- last one ----------------------------------------------------------------
	, { MessageType::ENUM_LENGTH				, "ENUM_LENGTH"					}
};
// EB - 20200924


struct Message
{
	unsigned int recipient;
	unsigned int sender;
	MessageType type;
	void * data;
};

class MessageBus : public EngineObjectBase 
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
		  { "audio"		, 3  }
	//	, { "camera"	, 4  }
	//	, { "cinematic"	, 5  }		
	//	, { "collision"	, 6  }
		, { "console"	, 7  }
	//	, { "debug"		, 8  }
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

	~MessageBus(){}

	unsigned int channel_lookup(char const * channel_name)
	{
		return name_to_channel_number[channel_name];
	}
	
	std::string channel_lookup(unsigned int id)
	{
		for (auto&& kvp : name_to_channel_number)
		{
			if (kvp.second == id)
			{
				return std::string(kvp.first);
			}
		}
		return "";
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

	// prints a <Message> to the engine log
	void log_message(Message m)
	{
			std::stringstream ss;

			ss << "Logging Message:\nMessage{";

			// sender
			if (m.sender > witchcraft::engine::engine_id_offset)
			{
				ss << "\n\tsender id: " << m.sender;
			}
			else
			{
				auto channel = channel_lookup(m.sender);
				if (channel == "")
				{
					ss << "\n\tsender id: " << m.sender;
				}
				else
				{
					ss << "\n\tsender channel: " << channel;
				}
			}

			// receiver
			if (m.recipient > witchcraft::engine::engine_id_offset)
			{
				ss << "\n\trecipient id: " << m.recipient;
			}
			else
			{
				auto channel = channel_lookup(m.recipient);
				if (channel == "")
				{
					ss << "\n\trecipient id: " << m.recipient;
				}
				else
				{
					ss << "\n\trecipient channel: " << channel;
				}
			}

			// type
			auto kvp = message_type_enum_to_string.find(m.type);
			if (kvp == message_type_enum_to_string.end())
			{
				ss << "\n\ttype id: " << m.type;
			}
			else
			{
				ss << "\n\ttype name: " << kvp->second;
			}

			// data
			ss << "\n\tdata: { " << m.data << " }";
			ss << "\n};";

			PLOGV << ss.str();
	}
};

#endif // !MESSAGE_BUS_H
