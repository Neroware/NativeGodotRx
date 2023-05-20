extends Node

# Called when the node enters the scene tree for the first time.

func _ready():
	self.__test_schedulers()
	self.__test_disposables()
	self._test_weakkeydict()



var n1 : Node
var n2 : Node
var n3 : Node
var n4 : Node
var c = 100
var dict : WeakKeyDictionary
func _test_weakkeydict():
	n1 = Node.new()
	n2 = Node.new()
	n3 = Node.new()
	n4 = Node.new()
	dict = WeakKeyDictionary.Get()
	dict.set_pair(n1, 1)
	dict.set_pair(n2, 2)
	dict.set_pair(n3, 3)
	dict.set_pair(n4, 4)
	#n1.queue_free()
	dict.erase(n2)
	dict.set_pair(n1, 0)
	#print(">>> ", dict.length)
	var foo = Disposable.Empty()
	dict.set_pair(foo, "I'm gonna die!")
	#print("Values> ", dict.values())
	#print("Keys> ", dict.keys())




func __test_schedulers():
	var scheduler = TrampolineScheduler.Get()
	for i in range(10):
		scheduler.schedule(func(__, ___): OS.delay_msec(10) ; print("!!!"), null)





func __test_disposables():
	var disp = Disposable.Get(func(): print(":)"))
	disp.dispose()
	
	var sad = SingleAssignmentDisposable.Get()
	sad.disposable = Disposable.Get(func(): print("1"))
	sad.dispose()
	
	var mad = MultiAssignmentDisposable.Get()
	mad.disposable = Disposable.Get(func(): print("1"))
	mad.disposable = Disposable.Get(func(): print("2"))
	mad.dispose()
	
	var cd = CompositeDisposable.Get([
		Disposable.Empty(),
		Disposable.Empty(),
		Disposable.Empty()
	])
	var d = Disposable.Get(func(): print("3"))
	cd.add(d)
	cd.add(Disposable.Get(func(): print("4")))
	cd.remove(d)
	if (cd.length == 4):
		cd.dispose()
	
	var inner = Disposable.Get(func(): print("5"))
	var rd = RefCountDisposable.Get(inner)
	var ref1 = rd.disposable
	var ref2 = rd.disposable
	ref1.dispose()
	ref2.dispose()
	rd.dispose()




# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var dt = RelativeTime.Get(0.0)
	var t0 = AbsoluteTime.Get(1.123)
	var t1 = AbsoluteTime.Get(0.023)
	var dt0 = t1.timedelta(t0)
#	
	c -= 1
	if (c == 0): 
		print("DELETE NODE")
		n1.queue_free()
	if (c == -100):
		dict.clear()
	print(">>> ", dict.keys())
#	print(">> ", dict.length)
