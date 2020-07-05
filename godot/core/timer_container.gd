extends Control

onready var label = get_node("PanelContainer/CenterContainer/Label")
var cnt = 0

func _ready():
	event_bus.subscribe("init_event", self, "_handle_init_event")

func _update_counter():
	label.text = String(cnt)

func _handle_init_event(data):
	cnt = 0
	_update_counter()

func _handle_increment_timer():
	cnt = cnt + 1
	_update_counter()
