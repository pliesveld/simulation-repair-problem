extends Control

func _ready():
	event_bus.publish("init_event", {
		"n": 10,
		"s": 4
	})
	
	event_bus.subscribe("crash_event", self, "_handle_crash_event")
	#$Timer.time_left = 10.0
	$Timer.wait_time = 10
	$Timer.start()

func _handle_crash_event(data):
	$Timer.stop()

func _on_Timer_timeout():
	event_bus.publish("failure_event", {})
