
#include "console/GDConsole.h"
#include "ExampleSimulation.h"
#include "console/FLConsole.h"

#include <core/Godot.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "console/gdconsole_sink.h"

using namespace godot;

bool ConsoleSimulationRestart( std::vector<std::string> *vArgs );

ExampleSimulation::ExampleSimulation() :
	m_nMachines(CVarUtils::CreateCVar<int>("simulation.machines", 8)),
	m_nSpares(CVarUtils::CreateCVar<int>("simulation.spares", 4)),
	m_nFailureTimer(CVarUtils::CreateCVar<int>("simulation.timer.failure", 1)),
	m_nRepairTimer(CVarUtils::CreateCVar<int>("simulation.timer.repair", 3))
{
	spdlog::cfg::load_env_levels();

	auto test_logger = spdlog::gdconsole_logger_st("console_logger");
	spdlog::set_default_logger(test_logger);
	test_logger->warn("this is a test");

	// examples
	spdlog::warn("Easy padding in numbers like {:08d}", 12);
	spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	spdlog::info("Support for floats {:03.2f}", 1.23456);
	spdlog::info("Positional args are {1} {0}..", "too", "supported");
	spdlog::info("{:>8} aligned, {:<8} aligned", "right", "left");

}

void ExampleSimulation::_init() {
	spdlog::info("ExampleSimulation::_init()\n");
	CVarUtils::CreateCVar( "restart", ConsoleSimulationRestart, "Restart Simulation" );
}

void ExampleSimulation::_ready() {
	godot::Godot::print("ExampleSimulation::_ready()\n");
}

bool ConsoleSimulationRestart( std::vector<std::string> *vArgs ) {
	// examples
	spdlog::warn("Easy padding in numbers like {:08d}", 12);
	spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	spdlog::info("Support for floats {:03.2f}", 1.23456);
	spdlog::info("Positional args are {1} {0}..", "too", "supported");
	spdlog::info("{:>8} aligned, {:<8} aligned", "right", "left");

	printf("Console Restart Command Called.");

	Node *node = GDConsole::get_singleton()->get_node("/root/event_bus");
	if( node ) {
		godot::Godot::print("node found");
		node->emit_signal("restart_signal", "restart");
	} else {
		godot::Godot::print("Node not found.");
	}
}






