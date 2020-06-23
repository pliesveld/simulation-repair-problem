extends Panel

onready var consoleLineEdit = get_node('../ConsoleLineEdit')

func _on_Panel_visibility_changed():
	consoleLineEdit.grab_focus()

