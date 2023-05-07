extends Node

var scheduler : Scheduler

# Called when the node enters the scene tree for the first time.
func _ready():
	var sd = SerialDisposable.Get()
	sd.disposable = Disposable.Get(func(): print("1"))
	sd.disposable = Disposable.Get(func(): print("2"))
	sd.disposable = Disposable.Get(func(): print("3"))
	sd.disposable = Disposable.Get(func(): print("4"))
	#sd.dispose()
	print(">>> ", sd.is_disposed)
	
	var rcd = RefCountDisposable.Get(sd)
	var dref1 = rcd.disposable
	var dref2 = rcd.disposable
	var dref3 = rcd.disposable
	print(">>> ", dref1.get_instance_id())
	print(">>> ", dref2.get_instance_id())
	print(">>> ", dref3.get_instance_id())
	print(">>> ", rcd.count)
	dref2.dispose()
	print(">>> ", rcd.count)
	#
	
	var sad = SingleAssignmentDisposable.Get()
	sad.disposable = Disposable.Empty()
	#sad.disposable = Disposable.Empty()
	
	scheduler = Scheduler.Get()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var t0 = AbsoluteTime.Get(1.123)
	var t1 = AbsoluteTime.Get(0.023)
	var dt0 = t1.timedelta(t0)
	print(">>> ", Scheduler.to_seconds(Scheduler.to_datetime(dt0)))
