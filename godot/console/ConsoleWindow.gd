extends Control

onready var consoleLineEdit = get_node("ConsoleView/MarginContainer/ConsoleContainer/ConsoleLineEdit")
onready var consoleRichTextLabel = get_node("ConsoleView/MarginContainer/ConsoleContainer/ConsolePanel/ConsoleRichTextLabel")
onready var consoleTransparentPanel = get_node("TransparentPanel")

# https://docs.godotengine.org/en/stable/tutorials/inputs/input_examples.html
# https://godotengine.org/qa/15055/button-to-call-new-window

func _ready():
	set_process_input(true)
	set_process(true)
	Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE);
# https://docs.godotengine.org/en/stable/classes/class_@globalscope.html#enum-globalscope-keylist

func _input(event):
	if event.is_action_pressed("ui_tilde"):
		if visible:
			_close_window()
			visible = not visible
		else:
			_open_window()
			visible = not visible
			
	if event.is_action_pressed("ui_up"):
		if visible:
			consoleRichTextLabel.HistoryBack()
			consoleLineEdit.set_deferred('caret_position', 1000)
			
			
	if event.is_action_pressed("ui_down"):
		if visible:
			consoleRichTextLabel.HistoryForward()
			consoleLineEdit.set_deferred('caret_position', 1000)

	if event is InputEventKey and event.pressed:
		if event.scancode == KEY_TAB:
			var s = consoleLineEdit.get_text()
			consoleRichTextLabel.TabComplete()
			
func _open_window():
	$Timer._on_Timer_windowOpen()

func _close_window():
	$Timer._on_Timer_windowClose()
