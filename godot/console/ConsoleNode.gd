extends Node2D

onready var panel = get_node("ConsoleContainer")
onready var consoleLineEdit = get_node("ConsoleContainer/ConsoleLineEdit")
onready var consoleRichTextLabel = get_node("ConsoleContainer/ConsolePanel/ConsoleRichTextLabel")

# https://docs.godotengine.org/en/stable/tutorials/inputs/input_examples.html
# https://godotengine.org/qa/15055/button-to-call-new-window

func _ready():
	set_process_input(true)
	set_process(true)
	Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE);
# https://docs.godotengine.org/en/stable/classes/class_@globalscope.html#enum-globalscope-keylist

func _input(event):
	if event is InputEventKey and event.pressed:
		global.simulation.ProcessKey(event.scancode)
		if event.scancode == KEY_QUOTELEFT:
			visible = not visible
			global.simulation._ready()
			print("KEY_QUOTELEFT was pressed")
		elif event.scancode == KEY_TAB:
			var s = consoleLineEdit.get_text()
			print("TODO: tab complete before: ", s)
			consoleRichTextLabel.tab_complete()
			consoleLineEdit.get_text()
			print("TODO: tab complete after: ", s)

# TODO: work in single invocation
# bool is_action_just_pressed ( String action ) const
# https://docs.godotengine.org/en/stable/classes/class_input.html#class-input-method-is-action-pressed

# or use InputMap
