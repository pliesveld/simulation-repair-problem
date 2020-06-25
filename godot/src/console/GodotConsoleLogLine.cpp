
#include "GodotConsoleLogLine.h"
#include <core/Godot.hpp>
#include <Reference.hpp>


#include <cassert>

using namespace godot;


GodotConsoleLogLine *GodotConsoleLogLine::_singleton = NULL;

//Engine *Engine::get_singleton() {
//	return singleton;
//}

GodotConsoleLogLine * GodotConsoleLogLine::get_singleton() {
	if (!GodotConsoleLogLine::_singleton) {
		GodotConsoleLogLine::_singleton = new GodotConsoleLogLine;
	}
	return GodotConsoleLogLine::_singleton;
}

GodotConsoleLogLine::GodotConsoleLogLine() {
}

void GodotConsoleLogLine::_init() {
	godot::Godot::print("GodotConsoleLogLine::_init() -- edit\n");
//	printf("_init\n");

	GodotConsoleLogLine::_singleton = this;
}

void GodotConsoleLogLine::_ready() {
	godot::Godot::print("GodotConsoleLogLine::_ready() -- edit\n");
//	printf("_ready\n");
//	printf("Finding lineEdit");

	m_LineEdit = (LineEdit *)get_node("../../ConsoleLineEdit");
	assert(m_LineEdit != nullptr);
}

void GodotConsoleLogLine::tab_complete() {
	{
		godot::String godotstr = m_LineEdit->get_text();
		m_Console.SetCurrentCommand(godotstr.utf8().get_data());
	}
	std::string newLineEdit = m_Console.TabComplete();

	if(newLineEdit != "") {
		godot::String godotstr(newLineEdit.c_str());
		m_LineEdit->set_text(godotstr);
		m_LineEdit->set_cursor_position(100000);
	}
}

void GodotConsoleLogLine::HistoryBack() {
	m_Console.HistoryBack();
	std::string cmd = m_Console.GetCurrentCommand();
	godot::String godotstr(cmd.c_str());
	m_LineEdit->clear();
	m_LineEdit->append_at_cursor(godotstr);
}

void GodotConsoleLogLine::HistoryForward() {
	m_Console.HistoryForward();
	std::string cmd = m_Console.GetCurrentCommand();
	godot::String godotstr(cmd.c_str());
	m_LineEdit->clear();
	m_LineEdit->append_at_cursor(godotstr);
}

void GodotConsoleLogLine::_test() {
	godot::Godot::print("GodotConsoleLogLine::_test()\n");
}

void GodotConsoleLogLine::_logline(const char *line) {
	godot::String godotstr(line);
	append_bbcode(godotstr);
}

void GodotConsoleLogLine::_command(String message) {
	Godot::print("Received command:");
	Godot::print(message);
	append_bbcode(message);
	newline();

	m_Console.handle2(message.utf8().get_data());
	m_LineEdit->clear();
}
