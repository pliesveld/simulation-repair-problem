extends LineEdit

# https://www.reddit.com/r/godot/comments/ch6zyp/how_to_focus_a_lineedit_node_from_script/
func _on_ConsoleLineEdit_visibility_changed():
	call_deferred('grab_focus')
