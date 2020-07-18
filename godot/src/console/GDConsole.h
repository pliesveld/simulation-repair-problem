#ifndef GODOT_CPP_GDCONSOLE_H
#define GODOT_CPP_GDCONSOLE_H

#include <core/Godot.hpp>
#include <gen/RichTextLabel.hpp>
#include <gen/LineEdit.hpp>

#include "FLConsole.h"

using namespace godot;

class GDConsole : public godot::RichTextLabel {
	GODOT_CLASS(GDConsole, godot::RichTextLabel
	);

	static GDConsole *_singleton;

	LineEdit *m_LineEdit;
	FLConsole m_Console;
	bool bInitialized = false;

public:
	static GDConsole *get_singleton();

	GDConsole();

	void _init();
	void _ready();

	void CommandEntered(String message);
	void LogLine(const char *line);
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

#endif
