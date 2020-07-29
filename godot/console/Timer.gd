extends Timer

onready var consoleTransparentPanel = get_node("../TransparentPanel")

# Timer used to control animation of opening console window
# The min height of TransparentPanel is used to 
# When min height is adjusted, the parent HBOXContainer will
# resize the ConsoleView window
#
# Adjust by changing the aspect ratio of TransparentPanel  
# 
# rect_min_size.y animates from [ 800 -> 0 ] * percent_open
#

# When animation completes, console should cover this percent of the window
const percent_open = 0.6

# state variables below for rendering animation of current timeout()
var orig_panel_y = 800
var px_delta = -1
var panel_y = 0

# number of iterations
# time to open is max_iteration * refresh_wait_time
const max_iteration = 5
var iteration = 1

# Timeout value when 
const refresh_wait_time = 0.01

func _ready():
	orig_panel_y = 800
	px_delta = (-1 * (orig_panel_y / max_iteration)) * percent_open
	

func _on_Timer_timeout():
	if iteration >= max_iteration:
		self.stop()
	iteration = iteration + 1
	var new_y = panel_y + (px_delta)
	consoleTransparentPanel.rect_min_size = Vector2(0, new_y)
	panel_y = new_y
	
func _on_Timer_windowOpen():
	self._reset()
	panel_y = orig_panel_y
	self.start(refresh_wait_time)
	
func _on_Timer_windowClose():
	self._reset()

func _reset():
	iteration = 1
	self.stop()
