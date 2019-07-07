extends Control

signal my_signal

onready var scene = preload("res://core/machine_working.tscn")
onready var n = 10

func _update_machines():
	var repair = $PanelContainer/VSplitContainer/Panel/CenterContainer
	
	if n > 0:
		var new_machine = scene.instance()
		var new_machine_animcation = new_machine.get_node("AnimatedSprite")
		new_machine_animcation.animation = "repairing"
		repair.add_child(new_machine)

	var grid = get_node("PanelContainer/VSplitContainer/GridContainer")
	for child in grid.get_children():
		grid.remove_child(child)

	for i in range(1, n):
		var new_machine = scene.instance()
		var new_machine_animcation = new_machine.get_node("AnimatedSprite")
		new_machine_animcation.animation = "broken"
		get_node("PanelContainer/VSplitContainer/GridContainer").add_child(new_machine)

func _ready():
	event_bus.subscribe("init_event", self, "_handle_init_event")
	event_bus.subscribe("crash_event", self, "_handle_crash_event")
	event_bus.subscribe("failure_event", self, "_handle_failure_event")	
	event_bus.subscribe("repair_event", self, "_handle_repair_event")	
	
func _handle_init_event(data):
	n = 0
	_update_machines()
	
func _handle_failure_event(data):
	n = n + 1
	_update_machines()

func _handle_repair_event(data):
	if n > 0:
		n = n - 1
		_update_machines()

func _handle_crash_event(data):
	n = n - 1
	_update_machines()	
