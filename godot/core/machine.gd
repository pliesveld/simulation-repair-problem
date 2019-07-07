extends StaticBody2D

enum MachineState {
  WORKING,
  SPARE,
  BROKEN
}

onready var state = MachineState.WORKING
onready var sprite = get_node('Sprite')


# Called when the node enters the scene tree for the first time.
func _ready():

	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

func _debug():
	print("The current MachineState:", MachineState.keys()[state])
	print("Current frame:", sprite.frame)
	print("Sprite:", sprite.modulate)
	


func _on_DebugButton_pressed():
	_debug()

func _on_BreakButton_pressed():
	print("Changing the machine state to BROKEN")
	state = MachineState.BROKEN
	sprite.frame = 2

func _on_RepairButton_pressed():
	print("Changing the machine state to SPARE")
	state = MachineState.SPARE
	sprite.frame = 1

func _on_ReplaceButton_pressed():
	print("Changing the machine state to WORKING")
	state = MachineState.WORKING
	sprite.frame = 0
