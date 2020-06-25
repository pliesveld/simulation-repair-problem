
#include "console/GodotConsoleLogLine.h"
#include "exampleSimulation.h"
#include "console/FLConsole.h"

#include <core/Godot.hpp>


using namespace godot;

bool ConsoleSimulationRestart( std::vector<std::string> *vArgs );


ExampleSimulation::ExampleSimulation() :
	m_nMachines(CVarUtils::CreateCVar<int>("simulation.machines", 8)),
	m_nSpares(CVarUtils::CreateCVar<int>("simulation.spares", 2)),
	m_nWaitTime(CVarUtils::CreateCVar<int>("simulation.wait_time", 10))
{

}

void ExampleSimulation::_init() {
	godot::Godot::print("ExampleSimulation::_init()\n");

	CVarUtils::CreateCVar( "restart", ConsoleSimulationRestart, "Restart Simulation" );
}

void ExampleSimulation::_ready() {
	godot::Godot::print("ExampleSimulation::_ready()\n");

}

int ExampleSimulation::get_machine_count() {
	return m_nMachines;
}

int ExampleSimulation::get_spare_count() {
	return m_nSpares;
}

int ExampleSimulation::get_wait_time() {
	return m_nWaitTime;
}

//
//void ExampleSimulation::restart() {
//	godot::Godot::print("ExampleSimulation::restart()\n");
//
//	emit_signal("test", "action");
////
////	Node *node = get_node("/root/event_bus");
////
////	if( node ) {
////		godot::Godot::print("node found");
////	} else {
////		godot::Godot::print("Node not found.");
////	}
//
//}

bool ConsoleSimulationRestart( std::vector<std::string> *vArgs ) {
	printf("Console Restart Command Called.");

	Node *node = GodotConsoleLogLine::get_singleton()->get_node("/root/event_bus");
	if( node ) {
		godot::Godot::print("node found");
	} else {
		godot::Godot::print("Node not found.");
	}

	node->emit_signal("restart_signal", "restart");

}


