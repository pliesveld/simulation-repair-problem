extends Control

signal my_signal

onready var scene = preload("res://core/machine_working.tscn")
onready var s = global.simulation.get_spare_count()

func _update_machines():
	var grid = get_node("PanelContainer/VSplitContainer/GridContainer")
	for child in grid.get_children():
		grid.remove_child(child)

	for i in range(0, s):
		var new_machine = scene.instance()
		var new_machine_animcation = new_machine.get_node("AnimatedSprite")
		new_machine_animcation.animation = "spare"
		get_node("PanelContainer/VSplitContainer/GridContainer").add_child(new_machine)

func _ready():
	event_bus.subscribe("init_event", self, "_handle_init_event")
	event_bus.subscribe("failure_event", self, "_handle_failure_event")
	event_bus.subscribe("repair_event", self, "_handle_repair_event")
	
func _handle_init_event(data):
	s = data.s
	_update_machines()
	pass
	
func _handle_failure_event(data):
	if s > 0:
		s = s - 1
		_update_machines()
	else:
		event_bus.publish("crash_event", {})

func _handle_repair_event(data):
	if s > 0:
		s = s + 1
		_update_machines()
