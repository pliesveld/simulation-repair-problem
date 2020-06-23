
#include "godotConsole.h"
#include <core/Godot.hpp>

using namespace godot;

GodotConsole::GodotConsole() {

}

void GodotConsole::_init() {
	godot::Godot::print("GodotConsole::_init() -- edit\n");
	printf("_init\n");
}

void GodotConsole::_ready() {
	godot::Godot::print("GodotConsole::_ready() -- edit\n");
	printf("_ready\n");
}

void GodotConsole::ProcessCommand(godot::Variant cmd) {
	Godot::print("Command: {0}", cmd);

}
