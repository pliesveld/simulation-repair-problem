extends RichTextLabel

signal my_signal

onready var consoleLineEdit = get_node('../../ConsoleLineEdit')

func _ready():
	pass


var linecount = 5
var cnt = 0
var thresh = 3

func _do_stuff(line):
	print('adding ' + line)
	append_bbcode(line)
#	add_text(line)
	newline()
	_do_buffer_updates()

func _do_buffer_updates():
	while(get_line_count ( ) > linecount):
		remove_line(0)

func _on_ConsoleLineEdit_text_entered(new_text):
	if new_text:
		_do_stuff(new_text)
		emit_signal("my_signal", new_text)
		global.simulation.ProcessCommand(new_text)
		consoleLineEdit.clear()
		
