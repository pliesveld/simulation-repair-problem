#ifndef GODOT_CPP_GDCONSOLE_H
#define GODOT_CPP_GDCONSOLE_H

#include <core/Godot.hpp>
#include <gen/RichTextLabel.hpp>
#include <gen/LineEdit.hpp>

#include "console/FLConsole.h"

using namespace godot;

class GDConsole : public godot::RichTextLabel {
	GODOT_CLASS(GDConsole, godot::RichTextLabel
	);

	static GDConsole *_singleton;

	LineEdit *m_LineEdit;
	FLConsole m_Console;

public:
	static GDConsole *get_singleton();

	GDConsole();

	void _init();
	void _ready();

	void CommandEntered(String message);
	void _logline(const char *line);
	void TabComplete();
	void HistoryBack();
	void HistoryForward();

	void EnterLogLine(const char *line, const LineProperty prop, bool display);

	static void _register_methods() {
		register_method("_init", &GDConsole::_init);
		register_method("_ready", &GDConsole::_ready);
		register_method("CommandEntered", &GDConsole::CommandEntered);
		register_method("TabComplete", &GDConsole::TabComplete);
		register_method("HistoryBack", &GDConsole::HistoryBack);
		register_method("HistoryForward", &GDConsole::HistoryForward);
	}
};


inline void FLConsoleInstance::EnterLogLine(const char *line, const LineProperty prop, bool display)
{
	_CheckInit();

	GDConsole *consoleLogLine = GDConsole::get_singleton();
	if(consoleLogLine == nullptr) {
		godot::Godot::print("Error, null singleton");
	} else {
		godot::String s(line);
		Godot::print(s);

		consoleLogLine->_logline(line);
		consoleLogLine->newline();

// script history
		if( (int)m_sConsoleText.size() >= m_nConsoleMaxHistory ) {
			m_sConsoleText.pop_back();
		}

		if( line != NULL ) {
			m_sConsoleText.push_front( ConsoleLine(std::string(line), prop, display) );

			if( m_bSavingScript && prop != LINEPROP_ERROR ) {
				m_ScriptText.push_front( ConsoleLine(std::string(line), prop, display) );
			}
		}
	}
//

}

#endif
