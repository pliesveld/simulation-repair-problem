#ifndef GODOT_CPP_EXAMPLESIMULATION_H
#define GODOT_CPP_EXAMPLESIMULATION_H

#include <core/Godot.hpp>
#include <Reference.hpp>

using namespace godot;

class ExampleSimulation : public godot::Reference {
	GODOT_CLASS(ExampleSimulation, godot::Reference
	);
public:
	ExampleSimulation();

	void _init();
	void _ready();


	static void _register_methods() {
		register_method("_init", &ExampleSimulation::_init);
		register_method("_ready", &ExampleSimulation::_ready);
		register_signal<ExampleSimulation>("command_signal", "string_argument", GODOT_VARIANT_TYPE_STRING);
		// register_signal<SimpleClass>("signal_name", "string_argument", GODOT_VARIANT_TYPE_STRING)

	}
};

#endif //GODOT_CPP_EXAMPLESIMULATION_H
