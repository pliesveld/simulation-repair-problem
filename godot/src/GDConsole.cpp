#include <cassert>

#include <core/Godot.hpp>
#include <Reference.hpp>

using namespace godot;

#include "GDConsole.h"

GDConsole *GDConsole::_singleton = NULL;

GDConsole * GDConsole::get_singleton() {
	if (!GDConsole::_singleton) {
		GDConsole::_singleton = new GDConsole;
	}
	return GDConsole::_singleton;
}

GDConsole::GDConsole() {
}

void GDConsole::_init() {
	godot::Godot::print("GDConsole::_init()\n");
	GDConsole::_singleton = this;
}

void GDConsole::_ready() {
	godot::Godot::print("GDConsole::_ready()\n");
	m_LineEdit = (LineEdit *)get_node("../../ConsoleLineEdit");
	assert(m_LineEdit != nullptr);
}

void GDConsole::TabComplete() {
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

void GDConsole::HistoryBack() {
	m_Console.HistoryBack();
	std::string cmd = m_Console.GetCurrentCommand();
	godot::String godotstr(cmd.c_str());
	m_LineEdit->clear();
	m_LineEdit->append_at_cursor(godotstr);
}

void GDConsole::HistoryForward() {
	m_Console.HistoryForward();
	std::string cmd = m_Console.GetCurrentCommand();
	godot::String godotstr(cmd.c_str());
	m_LineEdit->clear();
	m_LineEdit->append_at_cursor(godotstr);
}

void GDConsole::_logline(const char *line) {
	godot::String godotstr(line);
	append_bbcode(godotstr);
}

void GDConsole::_command(String message) {
	Godot::print("Received command:");
	Godot::print(message);
	append_bbcode(message);
	newline();

	m_Console.handle2(message.utf8().get_data());
	m_LineEdit->clear();
}
