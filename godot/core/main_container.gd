extends Control

func _ready():
	event_bus.subscribe("crash_event", self, "_handle_crash_event")
	event_bus.subscribe("init_event", self, "_handle_init_event")
	
	event_bus.publish("init_event", {
		"n": global.simulation.get_machine_count(),
		"s": global.simulation.get_spare_count()
	})
	
	
	
#	var rng = RandomNumberGenerator.new()
#	rng.set_seed(42)
#	print(rng.randi())
	

func _handle_crash_event(data):
	$Timer.stop()

func _handle_init_event(data):
	print("init wait time: ", global.simulation.get_wait_time())
	$Timer.wait_time = global.simulation.get_wait_time()
	$Timer.start()


func _on_Timer_timeout():
	event_bus.publish("failure_event", {})
