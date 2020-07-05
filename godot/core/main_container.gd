extends Control

onready var timer_container = get_node("HBoxContainer/Control4")

func _ready():
	event_bus.subscribe("crash_event", self, "_handle_crash_event")
	event_bus.subscribe("init_event", self, "_handle_init_event")
	
	event_bus.publish("init_event", {
		"n": global.simulation.get_machine_count(),
		"s": global.simulation.get_spare_count()
	})

	$FailureTimer.connect("timeout", timer_container, "_handle_increment_timer")
	$RepairTimer.connect("timeout", timer_container, "_handle_increment_timer")
	
#	var rng = RandomNumberGenerator.new()
#	rng.set_seed(42)
#   rng.randomize()
#	print(rng.randi())

func _handle_crash_event(data):
	$FailureTimer.stop()
	$RepairTimer.stop()

func _handle_init_event(data):
	$FailureTimer.start(global.simulation.get_next_failure_time())
	$RepairTimer.start(global.simulation.get_next_repair_time())

func _on_FailureTimer_timeout():
	event_bus.publish("failure_event", {})
	$FailureTimer.start(global.simulation.get_next_failure_time())

func _on_RepairTimer_timeout():
	event_bus.publish("repair_event", {})
	$RepairTimer.start(global.simulation.get_next_repair_time())
