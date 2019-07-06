extends Control

signal my_signal

onready var scene = preload("res://core/machine_working.tscn")
onready var n = 10

func _update_machines():
	var grid = get_node("PanelContainer/VSplitContainer/GridContainer")
	for child in grid.get_children():
		grid.remove_child(child)

	for i in range(0, n):
		var new_machine = scene.instance()
		var new_machine_animcation = new_machine.get_node("AnimatedSprite")
		new_machine_animcation.animation = "broken"
		get_node("PanelContainer/VSplitContainer/GridContainer").add_child(new_machine)

func _ready():
	_update_machines()
	connect("my_signal", self, "signal_handler")
	emit_signal("my_signal", 4)

func signal_handler(count):
	print("repair_container::signal_handler", count)
	n = count
	_update_machines()