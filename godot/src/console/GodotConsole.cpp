
#include "GodotConsole.h"
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

void GodotConsole::ProcessKey(int key) {
	Godot::print("ProcessKey: {0}", key);
//	m_Console.handle(key);
}

void GodotConsole::ProcessCommand(godot::Variant cmd) {
	Godot::print("ProcessCommand: {0}", cmd);
}
