
#include "console/GodotConsoleLogLine.h"
#include "exampleSimulation.h"
#include "console/FLConsole.h"

#include <core/Godot.hpp>


using namespace godot;

bool ConsoleSimulationRestart( std::vector<std::string> *vArgs );

ExampleSimulation::ExampleSimulation() :
	m_nMachines(CVarUtils::CreateCVar<int>("simulation.machines", 8)),
	m_nSpares(CVarUtils::CreateCVar<int>("simulation.spares", 4)),
	m_nFailureTimer(CVarUtils::CreateCVar<int>("simulation.timer.failure", 1)),
	m_nRepairTimer(CVarUtils::CreateCVar<int>("simulation.timer.repair", 3))
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

int ExampleSimulation::get_next_failure_time() {
	return m_nFailureTimer;
}

int ExampleSimulation::get_next_repair_time() {
	return m_nRepairTimer;
}

bool ConsoleSimulationRestart( std::vector<std::string> *vArgs ) {
	printf("Console Restart Command Called.");

	Node *node = GodotConsoleLogLine::get_singleton()->get_node("/root/event_bus");
	if( node ) {
		godot::Godot::print("node found");
		node->emit_signal("restart_signal", "restart");
	} else {
		godot::Godot::print("Node not found.");
	}
}


