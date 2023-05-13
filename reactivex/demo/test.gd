extends Node

# Called when the node enters the scene tree for the first time.

var w1 = Thread.new()
var w2 = Thread.new()

func _ready():
	var rlk = Lock.Get()
	
	var _worker1 = func():
		rlk.lock()
		print("W1 aquired lock")
		
		rlk.lock()
		print("W1 aquired lock")
		
		OS.delay_msec(1000)
		
		rlk.unlock()
		print("W1 released lock")
	
	var _worker2 = func():
		OS.delay_msec(2000)
		
		rlk.unlock()
		print("W2 released lock")
	
	w1.start(_worker1)
	w2.start(_worker2)
	
	get_tree().create_timer(10).timeout.connect(func(): w1.wait_to_finish() ; w2.wait_to_finish())
