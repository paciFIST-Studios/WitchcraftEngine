#include "console.h"

void Console::add_default_commands()
{
	// sets all values in buffer to 0
	memset(input_buffer, 0, sizeof(input_buffer));

	commands.push_back("HELP");
	commands.push_back("HISTORY");
	commands.push_back("CLEAR");
	commands.push_back("CLOSE");
}

void Console::handle_message(Message m)
{
	if (m.type != MessageType::INVOKE__CONSOLE_COMMAND) { return; }

	auto command_str = static_cast<std::string*>(m.data);

	if (contains_term(command_str, "draw_console="))
	{
		if (contains_term(command_str, "true"))
		{
			draw_console = true;
		}
		else if (contains_term(command_str, "false"))
		{
			draw_console = false;
		}
		else if (contains_term(command_str, "toggle"))
		{
			draw_console = !draw_console;
		}
	}

	// OTHER COMMANDS
}

void Console::execute_command(char const * command)
{
	std::stringstream ss;
	ss << "# " << command << "\n";
	contents.push_back(ss.str());

	int constexpr STRCMP_TRUE = 0;
	std::string cmd(command);

	if (cmd.compare("CLEAR") == STRCMP_TRUE)
	{
		contents.clear();
	}
	else if (cmd.compare("HELP") == STRCMP_TRUE)
	{
		contents.push_back("Commands: ");
		for (auto cmd : commands)
		{
			ss = std::stringstream();
			ss << "- " << cmd;
			contents.push_back(ss.str());
		}
	}
	else if (cmd.compare("HISTORY") == STRCMP_TRUE)
	{
		// todo
	}
	else if (cmd.compare("CLOSE") == STRCMP_TRUE)
	{
		draw_console = false;
	}
	else
	{
		ss = std::stringstream();
		ss << "Unknown command: " << command << "\n";
		contents.push_back(ss.str());
	}

	scroll_to_bottom = true;
}

int Console::text_edit_callback_stub(ImGuiInputTextCallbackData * data)
{
	Console * c = (Console*)data->UserData;
	return c->text_edit_callback(data);
}

int Console::text_edit_callback(ImGuiInputTextCallbackData * data)
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

void Console::init_component()
{
	// add additional commands
}

void Console::draw(std::string title)
{
	if (!draw_console) { return; }

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
		contents.push_back("debug text");
	}

	ImGui::SameLine();

	if (ImGui::SmallButton("Add Debug Error"))
	{
		contents.push_back("[error] error text");
	}

	ImGui::SameLine();

	if (ImGui::SmallButton("Add debug comment"))
	{
		contents.push_back("# comment mark");
	}

	ImGui::SameLine();

	if (ImGui::SmallButton("Clear"))
	{
		contents.clear();
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
			contents.clear();
		}
		ImGui::EndPopup();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));


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