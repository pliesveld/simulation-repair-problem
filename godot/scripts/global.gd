extends Node

var simulation = preload("../demo/bin/gdexample.gdns").new()

func _ready():
	pass






#https://godotengine.org/qa/18552/gdnative-convert-godot-string-to-const-char
#func _ready():
#var gdn = GDNative.new()
#gdn.library = load("res://lib/sqllib.tres")
#gdn.initialize()
#gdn.call_native("standard_varcall", "query", [])
#gdn.terminate()
