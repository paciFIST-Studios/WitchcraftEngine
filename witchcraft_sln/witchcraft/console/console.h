#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>
#include <string>

#include "../engine/engine_object.h"
#include "../message_bus/message_bus.h"

#include "../imgui/imgui.h"

class MessageBus;

class Console : public qEngineObject
{
private:
protected:

	MessageBus * message_bus = nullptr;

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

	void add_default_commands()
	{
		// sets all values in buffer to 0
		memset(input_buffer, 0, sizeof(input_buffer));

		commands.push_back("HELP");
		commands.push_back("HISTORY");
		commands.push_back("CLEAR");
		commands.push_back("CLOSE");
	}

public:
	Console()
	{
		add_default_commands();
	}

	Console(MessageBus * mb) : message_bus(mb)
	{
		add_default_commands();
	}

	~Console() {}
	
	std::vector<std::string> const & get_commands() { return commands; }
	std::vector<std::string> const & get_contents() { return contents; }

	bool get_is_visible() { return draw_console; }
	void set_is_visible(bool is) { draw_console = is; }
	void toggle_visibility() { draw_console = !draw_console; }

	void clear_contents()
	{
		contents.clear();
	}
	
	void add_log(std::string s)
	{
		contents.push_back(s);
	}
	
	void draw(std::string title)
	{
		ImGui::SetNextWindowSize(
			ImVec2(
				200, 100
			)
			, ImGuiCond_FirstUseEver
		);
				
		if (!ImGui::Begin(title.c_str(), &draw_console))
		{
			ImGui::End();
			return;
		}
		
		ImGui::TextWrapped("HELP for help; [Tab] to use text completion");
		
		if (ImGui::SmallButton("Add Debug Text"))
		{
			add_log("debug text");
		}
		
		ImGui::SameLine();
		
		if (ImGui::SmallButton("Add Debug Error"))
		{
			add_log("[error] error text");
		}
		
		ImGui::SameLine();

		if (ImGui::SmallButton("Add debug comment"))
		{
			add_log("# comment mark");
		}

		ImGui::SameLine();
		
		if (ImGui::SmallButton("Clear"))
		{
			clear_contents();
		}
		
		ImGui::Separator();
		
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto Scroll", &use_auto_scroll);
			ImGui::EndPopup();
		}
		
		if (ImGui::Button("Options"))
		{
			ImGui::OpenPopup("Options");
		}
		ImGui::SameLine();
		filter.Draw("Find (\"incl\", \"-excl\")", 180.f);
		ImGui::Separator();
		
		const float footer_height_to_reserve = ImGui::GetStyle().ItemInnerSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("Scrolling Region", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear")) 
			{
				clear_contents();
			}
			ImGui::EndPopup();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1));

		
		for (auto && item : contents)
		{
			if (!filter.PassFilter(item.c_str()))
			{
				continue;
			}

			ImVec4 color;
			bool use_color = false;

			if (item.find(ERROR_TAG) != std::string::npos)
			{
				color = { 1.0f, 0.4f, 0.4f, 1.0f };
				use_color = true;
			}
			else if (strncmp(item.c_str(), COMMENT_MARK, 2) == 0)
			{
				color = { 1.0f, 0.8f, 0.6f, 1.0f };
				use_color = true;
			}

			if (use_color)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			}

			ImGui::TextUnformatted(item.c_str());

			if (use_color)
			{
				ImGui::PopStyleColor();
			}
		}

		if (use_auto_scroll && (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
		{
			ImGui::SetScrollHereY(1.0f);
		}
		
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		bool reclaim_focus = false;
		ImGuiInputTextFlags input_text_flags = (
			  ImGuiInputTextFlags_EnterReturnsTrue 
			| ImGuiInputTextFlags_CallbackCompletion 
			| ImGuiInputTextFlags_CallbackHistory
		);
		if (ImGui::InputText(
				"Input"
				, input_buffer
				, sizeof(input_buffer)
				, input_text_flags
				, &text_edit_callback_stub
				, (void*)this
		))
		{
			char * s = input_buffer;
			Strtrim(s);
			if (s[0]) 
			{	
				execute_command(s); 
			}
			
			for (int i = 0; i < 256; i++)
			{
				input_buffer[i] = '\0';
			}
			
			reclaim_focus = true;
		}
		
		ImGui::SetItemDefaultFocus();
		
		if (reclaim_focus)
		{
			// focus previous
			ImGui::SetKeyboardFocusHere(-1);
		}

		ImGui::End();
	}


	void execute_command(char const * command)
	{
		std::stringstream ss;
		ss << "# " << command << "\n";
		add_log(ss.str());

		if (_stricmp(command, "CLEAR") == 0)
		{
			clear_contents();
		}
		else if (_stricmp(command, "HELP") == 0)
		{
			add_log("Commands: ");
			for (auto cmd : commands)
			{
				ss = std::stringstream();
				ss << "- " << cmd;
				add_log(ss.str());
			}
		}
		else if (_stricmp(command, "HISTORY") == 0)
		{
			// todo
		}
		else if (_stricmp(command, "CLOSE") == 0)
		{
			draw_console = false;
		}
		else
		{
			ss = std::stringstream();
			ss << "Unknown command: " << command << "\n";
			add_log(ss.str());
		}

		scroll_to_bottom = true;
	}

	static int text_edit_callback_stub(ImGuiInputTextCallbackData * data)
	{
		Console * c = (Console*)data->UserData;
		return c->text_edit_callback(data);
	}

	int text_edit_callback(ImGuiInputTextCallbackData * data)
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
		{
			// see: https://github.com/ocornut/imgui/blob/docking/imgui_demo.cpp
		}
		else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
		{

		}

		return 0;
	}

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
