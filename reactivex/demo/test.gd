extends Node

# Called when the node enters the scene tree for the first time.

func _ready():
	pass
#	RxLambda.Test()
#	self.__test_schedulers()
#	self.__test_disposables()
#	self.__test_weakkeydict()
#	self.__test_threads()
#	self.__test_currthreadscheduler()



var n1 : Node
var n2 : Node
var n3 : Node
var n4 : Node
var c = 100
var dict : WeakKeyDictionary
func __test_weakkeydict():
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
	var foo = Disposable.Get()
	dict.set_pair(foo, "I'm gonna die!")
	#print("Values> ", dict.values())
	#print("Keys> ", dict.keys())




func __test_schedulers():
	var scheduler = TrampolineScheduler.Get()
	for i in range(10):
		scheduler.schedule(func(__, ___): OS.delay_msec(10) ; print("!!!"))
	
	var cts = CurrentThreadScheduler.singleton()
	assert(cts == CurrentThreadScheduler.singleton())
	cts.schedule(func(__, ___): print("Scheduled on main thread!"))
	
	var ims = ImmediateScheduler.singleton()
	assert(ims == ImmediateScheduler.singleton())
	ims.schedule(func(__, ___): print("Scheduled immediatly!"))
	
	#var tup = GDRx.tuple("1", "2", "3")
	#print(">> ", tup)
	
	GDRx.enumerate({1:"A", 2:"B", 3:"C"}, func(i, pair): print(i, "> ", pair))
	#GDRx.enumerate(tup, func(i1, i2): print(">>> ", GDRx.tuple(i1, i2)))
	#var it = DictionaryIterator.Get({1:"A", 2:"B", 3:"C"})
	#print(">> ", it.front())
	#GDRx.foreach({1:"A", 2:"B", 3:"C"}, func(pair): print("> ", pair))
	#var it = ArrayIterator.Get(["A", "B", "C"])
	#it.enumerate(func(i, item): print("> ", i, "; ", item))
	#GDRx.foreach({"1": "A", "2": "B", "3": "C"}, func(__ = null, ___ = null): print("!"))




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
		Disposable.Get(),
		Disposable.Get(),
		Disposable.Get()
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


func __test_threads():
	
	var threads : Array[RxThread]
	for i in range(1000):
		var t = RxThread.Get()
		threads.push_back(t)
		t.start(func(): return RxThread.get_current_thread() == t, Thread.PRIORITY_NORMAL)
	
	for i in range(1000):
		assert(threads[i].wait_to_finish())
	print(":)")
	print(">>> ", RxThread.get_current_thread() is RxMainThread)


func __test_currthreadscheduler():
	var cts = CurrentThreadScheduler.singleton()
	print(">> ", cts.get_instance_id())
	var threads : Array[RxThread]
	for i in range(100):
		var t = RxThread.Get()
		threads.push_back(t)
		t.start(func(): print(i, ">>> ", CurrentThreadScheduler.singleton().get_instance_id()), Thread.PRIORITY_NORMAL)
	
	for i in range(100):
		threads[i].wait_to_finish()


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	var dt = RelativeTime.Get(0.0)
#	var t0 = AbsoluteTime.Get(1.123)
#	var t1 = AbsoluteTime.Get(0.023)
#	var dt0 = t1.timedelta(t0)
##	
#	c -= 1
#	if (c == 0): 
#		print("DELETE NODE")
#		n1.queue_free()
#	if (c == -100):
#		dict.clear()
#	print(">>> ", dict.keys())
#	print(">> ", dict.length)
