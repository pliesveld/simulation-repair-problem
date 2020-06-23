
#include "exampleSimulation.h"
#include "console/FLConsole.h"


#include <core/Godot.hpp>

using namespace godot;

ExampleSimulation::ExampleSimulation() {

}

void ExampleSimulation::_init() {
	godot::Godot::print("_init() -- edit\n");
	printf("_init\n");
}

void ExampleSimulation::_ready() {
	godot::Godot::print("_ready() -- edit\n");
	printf("_ready\n");
}
