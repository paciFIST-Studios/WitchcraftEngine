#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>
#include <string>

#include "../engine/engine_object.h"
#include "../message_bus/message_bus.h"

#include "../imgui/imgui.h"


enum class EConsoleState : unsigned char
{
	// completely uninitialized
	  UNINIT
	// ctor has been called
	, CONSTRUCTED
	// has ptr to message bus, and ! nullptr
	, CONNECTED
	// can send message succcessfully
	, CAN_SEND
	// can receive message successfully
	, CAN_RECEIVE
	// is set to draw self to screen
	, DRAWING
};

class Console : public EngineObjectBase
{
private:
protected:

	MessageBus * message_bus = nullptr;

	EConsoleState current_console_state = EConsoleState::UNINIT;

	char const * COMMENT_MARK = "# ";
	char const * ERROR_TAG = "[error]";

	char input_buffer[256];

	std::vector<std::string> contents;
	std::vector<std::string> commands;
	std::vector<std::string> history;
	int position_in_history;
		
	ImGuiTextFilter filter;
	
	bool draw_console = false;
	bool use_auto_scroll = true;
	bool scroll_to_bottom = false;
	
	inline bool contains_term(std::string const * source, char const * search)
	{
		return source->find(search) != std::string::npos;
	}

	//inline std::string trim_in_place(std::string & str)
	//{
	//	// https://stackoverflow.com/a/6500499
	//	char const * WS = " \t\r\v\n";
	//	str.erase(0, str.find_first_not_of(WS));
	//	str.erase(str.find_last_not_of(WS)+1);
	//	return str;
	//}

	static void  Strtrim(char* s) 
	{ 
		char* str_end = s + strlen(s);
		while (str_end > s && str_end[-1] == ' ') str_end--; 
		*str_end = 0; 
	}

	void add_default_commands();
	
	void handle_message(Message m);
	
	void execute_command(char const * command);
	
	static int text_edit_callback_stub(ImGuiInputTextCallbackData * data);
	
	int text_edit_callback(ImGuiInputTextCallbackData * data);

public:
	Console() 
	: EngineObjectBase()
	{
		add_default_commands();
		current_console_state = EConsoleState::CONSTRUCTED;
	}

	Console(MessageBus * mb) 
	: EngineObjectBase()
	, message_bus(mb)
	{
		add_default_commands();

		std::function<void(Message)> cb = std::bind(&Console::handle_message, this, std::placeholders::_1);
		message_bus->subscribe("console", cb);
	}

	~Console() {}

	void init_component();

	EConsoleState get_console_state() const { return current_console_state; }
	
	void draw(std::string title);

};

namespace witchcraft
{
	namespace console
	{
		int constexpr INPUT_BUFFER_SIZE = 256;
		float constexpr console_w = 200.f;
		float constexpr console_h = 100.f;
	}
}

#endif // !CONSOLE_H
