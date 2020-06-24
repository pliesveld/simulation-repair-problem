#ifndef GODOT_CPP_GODOTCONSOLE_H
#define GODOT_CPP_GODOTCONSOLE_H

#include <core/Godot.hpp>
#include <Reference.hpp>
//#include "FLConsole.h"

using namespace godot;

class GodotConsole : public godot::Reference {
	GODOT_CLASS(GodotConsole, godot::Reference
	);

//	FLConsole m_Console;

public:
	GodotConsole();

	void _init();
	void _ready();
	void ProcessCommand(Variant cmd);
	void ProcessKey(int key);


	static void _register_methods() {
		register_method("_init", &GodotConsole::_init);
		register_method("_ready", &GodotConsole::_ready);
		register_method("ProcessCommand", &GodotConsole::ProcessCommand);
		register_method("ProcessKey", &GodotConsole::ProcessKey);
		register_signal<GodotConsole>("command_signal", "string_argument", GODOT_VARIANT_TYPE_STRING);
	}


};

#endif
