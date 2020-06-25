#ifndef GODOT_CPP_EXAMPLESIMULATION_H
#define GODOT_CPP_EXAMPLESIMULATION_H

#include <core/Godot.hpp>
#include <Reference.hpp>

using namespace godot;

class ExampleSimulation : public godot::Reference {
	GODOT_CLASS(ExampleSimulation, godot::Reference
	);


	// Note: type is int &
	int &m_nMachines;
	int &m_nSpares;
	int &m_nWaitTime;

public:
	ExampleSimulation();

	void _init();
	void _ready();

	int get_machine_count();
	int get_spare_count();
	int get_wait_time();


	static void _register_methods() {
		register_method("_init", &ExampleSimulation::_init);
		register_method("_ready", &ExampleSimulation::_ready);

		register_method("get_machine_count", &ExampleSimulation::get_machine_count);
		register_method("get_spare_count", &ExampleSimulation::get_spare_count);
		register_method("get_wait_time", &ExampleSimulation::get_wait_time);

		register_signal<ExampleSimulation>("restart_signal", "string_argument", GODOT_VARIANT_TYPE_STRING);

	}
};




#endif //GODOT_CPP_EXAMPLESIMULATION_H
