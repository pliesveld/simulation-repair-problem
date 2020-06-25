extends Control

signal my_signal

onready var scene = preload("res://core/machine_working.tscn")
onready var grid = get_node("PanelContainer/VSplitContainer/GridContainer")
onready var n = global.simulation.get_machine_count()

func _update_machines():

	for child in grid.get_children():
		grid.remove_child(child)

	for i in range(0, n):
		var new_machine = scene.instance()
		var new_machine_animcation = new_machine.get_node("AnimatedSprite")
		new_machine_animcation.animation = "working"
		grid.add_child(new_machine)

func _halt_machines():
	for child in grid.get_children():
		var sprite = child.get_node("AnimatedSprite")
		sprite.playing = false
		sprite.frame = 0

func _ready():
	event_bus.subscribe("init_event", self, "_handle_init_event")
	event_bus.subscribe("crash_event", self, "_handle_crash_event")
	#event_bus.subscribe("failure_event", self, "_handle_failure_event")	
	
func _handle_init_event(data):
	n = data.n
	print("control", data)
	print(n)
	_update_machines()	
	
func _handle_crash_event(data):
	_halt_machines()

#func _handle_failure_event(data):
#	if n > 0:
#		n = n - 1
#		_update_machines()
