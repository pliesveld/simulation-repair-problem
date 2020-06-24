#ifndef GODOT_CPP_GODOTCONSOLELOGLINE_H
#define GODOT_CPP_GODOTCONSOLELOGLINE_H

#include <core/Godot.hpp>
#include <gen/RichTextLabel.hpp>
#include <gen/LineEdit.hpp>
#include <Reference.hpp>



#include "FLConsole.h"


using namespace godot;

class GodotConsoleLogLine : public godot::RichTextLabel {
	GODOT_CLASS(GodotConsoleLogLine, godot::RichTextLabel
	);

	static GodotConsoleLogLine *_singleton;


	LineEdit *m_LineEdit;
//	Node *m_LineEdit;
	FLConsole m_Console;



public:
	static GodotConsoleLogLine *get_singleton();

	GodotConsoleLogLine();

	void _init();
	void _ready();
	void _command(String message);
	void _logline(const char *line);

	void tab_complete();
	void _test();


	void EnterLogLine(const char *line, const LineProperty prop, bool display);

	static void _register_methods() {
		register_method("_init", &GodotConsoleLogLine::_init);
		register_method("_ready", &GodotConsoleLogLine::_ready);
		register_method("tab_complete", &GodotConsoleLogLine::tab_complete);
		register_method("_test", &GodotConsoleLogLine::_test);
		register_method("_command", &GodotConsoleLogLine::_command);
		register_signal<GodotConsoleLogLine>("_command", "string_argument", GODOT_VARIANT_TYPE_STRING);
	}


};


inline void FLConsoleInstance::EnterLogLine(const char *line, const LineProperty prop, bool display)
{
	_CheckInit();

	GodotConsoleLogLine *consoleLogLine = GodotConsoleLogLine::get_singleton();
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
