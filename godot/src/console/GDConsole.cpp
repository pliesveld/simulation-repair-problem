#include <cassert>

#include <core/Godot.hpp>
#include <Reference.hpp>

using namespace godot;

#include "GDConsole.h"

GDConsole *GDConsole::_singleton = NULL;

GDConsole * GDConsole::get_singleton() {
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
	bInitialized = true;
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

void GDConsole::LogLine(const char *line) {
	godot::String godotstr(line);

	if(!bInitialized) {
		godot::Godot::print(godotstr);
	} else {
		append_bbcode(godotstr);
	}
}

void GDConsole::CommandEntered(String message) {
	m_Console.handleCommandEntered(message.utf8().get_data());
	m_LineEdit->clear();
}

void FLConsoleInstance::EnterLogLine(const char *line, const LineProperty prop, bool display)
{
	_CheckInit();

	GDConsole *consoleLogLine = GDConsole::get_singleton();
	if(consoleLogLine == nullptr)
	{
		godot::Godot::print("Error, null singleton");
	} else {
		if(display)
		{
			consoleLogLine->LogLine(line);
			consoleLogLine->newline();
		}
// script history
		if( (int)m_sConsoleText.size() >= m_nConsoleMaxHistory )
		{
			m_sConsoleText.pop_back();
		}

		if( line != NULL )
		{
			m_sConsoleText.push_front( ConsoleLine(std::string(line), prop, display) );

			if( m_bSavingScript && prop != LINEPROP_ERROR )
			{
				m_ScriptText.push_front( ConsoleLine(std::string(line), prop, display) );
			}
		}
	}
//

}
