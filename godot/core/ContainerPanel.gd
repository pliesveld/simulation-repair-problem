extends GridContainer

func _ready():
	print(self.get_script().get_path().get_base_dir())
	
	print(get_tree().current_scene)
	print(get_tree().edited_scene_root)
	if (get_tree().edited_scene_root):
		print(get_tree().edited_scene_root.filename)

	print(get_path())
	print(find_node('machine', true, true))
	# get_node("Node2D/machine2/Sprite").frame = 2	
	pass # Replace with function body.






func _on_PanelContainer2_sort_children():
	pass # Replace with function body.
