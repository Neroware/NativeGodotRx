extends "res://test_base.gd"

var custom_signal_emitted = null

func _ready():
#	var scheduler = RxScheduler.CurrentThreadSchedulerSingleton()
#	
#	RxObservable.catch([
#		RxObservable.raise("Planned exception!", scheduler),
#		RxObservable.raise("Planned exception 2!", scheduler),
#		RxObservable.just(42, scheduler),
#		RxObservable.just(123, scheduler)
#	]) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print(":)")) \
#	.dispose_with(self)
#
#	var obs_defer = RxObservable.defer(func(__ : RxSchedulerBase): return RxObservable.just("Deferred!"))
#	obs_defer.subscribe(func(i): print("i> ", i)).dispose_with(self)
#
#	RxObservable.case(
#		func(): return "foo", 
#		{
#			"bar": RxObservable.just(":("), 
#			"foo": RxObservable.just(":)")
#		}, 
#		RxObservable.raise("Should not happen!")
#	) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
#	.dispose_with(self)
#
#	RxObservable.combine_latest([
#		RxObservable.just(1),
#		RxObservable.just(2),
#		RxObservable.just(3)
#	]) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
#	.dispose_with(self)
#
#	RxObservable.concat([
#		RxObservable.just("Al"),
#		RxObservable.just("oh"),
#		RxObservable.just("a!")
#	]) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
#	.dispose_with(self)
#
#	RxObservable.fork_join([
#		RxObservable.just("Al"),
#		RxObservable.just("oh"),
#		RxObservable.just("a!")
#	]) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
#	.dispose_with(self)
#
#	RxObservable.from(["ichi", "ni", "san", "shi"]) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
#	.dispose_with(self)
#
#	RxObservable.if_then(
#		func(): return true,
#		RxObservable.just(":)"),
#		RxObservable.just(":(")
#	) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
#	.dispose_with(self)
#
#	RxObservable.on_error_resume_next([
#		RxObservable.just(1),
#		RxObservable.raise("Intentional Error"),
#		RxObservable.from([2, 3, 4])
#	]) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
#	.dispose_with(self)
#
#	RxObservable.on_error_resume_next([
#		func(err : RxError): return RxObservable.just(0),
#		func(err : RxError): return RxObservable.raise("Intentional Error"),
#		func(err : RxError): print("Produce with prev. err: ", err) ; return RxObservable.just(1),
#		func(err : RxError): return RxObservable.from([2, 3, 4])
#	], true) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
#	.dispose_with(self)
#
#	RxObservable.range(0, 10, 3) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
#	.dispose_with(self)
#
#	RxScheduler.NewThreadScheduler().schedule_periodic(
#		RelativeTime.from_seconds(1.0), 
#		func(__ = null): print("Periodic: ", OS.get_thread_caller_id()),
#	) \
#	.dispose_with(self)
	
#	RxScheduler.NewThreadScheduler().schedule_relative(
#		RelativeTime.from_seconds(3.0), 
#		func(__ = null, ___ = null): print("Scheduled on separate thread!")
#	) \
#	.dispose_with(self)
	
#	var nts = RxScheduler.NewThreadScheduler()
#	for i in range(1000):
#		nts.schedule_relative(
#			RelativeTime.from_seconds(randi() % 3),
#			func(__ = null, ___ = null): print("Scheduled on thread: ", OS.get_thread_caller_id())
#		) \
#		.dispose_with(self)
	
	var tos = RxSceneTreeTimeoutScheduler.singleton()
	tos.schedule_relative(
		RelativeTime.from_seconds(3), 
		func(__ = null, ___ = null): print("Scheduled timeout!")
	) \
	.dispose_with(self)
#	
#	RxObservable.timer(3.0, 1.0) \
#		.subscribe(func(i): print("Timeout!")) \
#		.dispose_with(self)
	
#	RxObservable.raise("This is an intentional error", "IntentionalError") \
#		.subscribe(func(__): pass, func(e): print("ERR: ", e)) \
#		.dispose_with(self)
	
#	RxObservable.with_latest_from(RxObservable.timer(1.0), [
#		RxObservable.timer(0.0, 0.1),
#		RxObservable.just(42),
#		RxObservable.from_iterable([1, 2, 3, 4]),
#	]) \
#	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
#	.dispose_with(self)
	
	RxObservable.zip([
		RxObservable.periodic_timer(0.5),
		RxObservable.periodic_timer(1.0),
		RxObservable.periodic_timer(2.0)
	]) \
	.subscribe(func(i): print("i> ", i), func(e): print("ERR: ", e), func(): print("END")) \
	.dispose_with(self)
	
	var disp = RxSingleAssignmentDisposable.get()
	disp.disposable = RxDisposable.get(func(): print("Disposed"))
	disp.dispose()
	
	var els = RxEventLoopScheduler.get()
	var els_disp : RxDisposableBase = RxDisposableBase.dyn_cast(els)
	var els_obs : RxObservable = RxObservable.dyn_cast_or_null(els)
	print(">>> ", els_disp)
	var scheduler : RxScheduler = RxScheduler.dyn_cast_or_null(els)
	print(">>> ", scheduler)
	print(">>> ", RxDisposableBase.dyn_cast_or_null(scheduler))
	
#	var example: Example = $Example
#
#	# Signal.
#	example.emit_custom_signal("Button", 42)
#	assert_equal(custom_signal_emitted, ["Button", 42])
#
#	# To string.
#	assert_equal(example.to_string(),'Example:[ GDExtension::Example <--> Instance ID:%s ]' % example.get_instance_id())
#	# It appears there's a bug with instance ids :-(
#	#assert_equal($Example/ExampleMin.to_string(), 'ExampleMin:[Wrapped:%s]' % $Example/ExampleMin.get_instance_id())
#
#	# Call static methods.
#	assert_equal(Example.test_static(9, 100), 109);
#	# It's void and static, so all we know is that it didn't crash.
#	Example.test_static2()
#
#	# Property list.
#	example.property_from_list = Vector3(100, 200, 300)
#	assert_equal(example.property_from_list, Vector3(100, 200, 300))
#
#	# Call simple methods.
#	example.simple_func()
#	assert_equal(custom_signal_emitted, ['simple_func', 3])
#	example.simple_const_func()
#	assert_equal(custom_signal_emitted, ['simple_const_func', 4])
#
#	# Pass custom reference.
#	assert_equal(example.custom_ref_func(null), -1)
#	var ref1 = ExampleRef.new()
#	ref1.id = 27
#	assert_equal(example.custom_ref_func(ref1), 27)
#	ref1.id += 1;
#	assert_equal(example.custom_const_ref_func(ref1), 28)
#
#	# Pass core reference.
#	assert_equal(example.image_ref_func(null), "invalid")
#	assert_equal(example.image_const_ref_func(null), "invalid")
#	var image = Image.new()
#	assert_equal(example.image_ref_func(image), "valid")
#	assert_equal(example.image_const_ref_func(image), "valid")
#
#	# Return values.
#	assert_equal(example.return_something("some string"), "some string42")
#	assert_equal(example.return_something_const(), get_viewport())
#	var null_ref = example.return_empty_ref()
#	assert_equal(null_ref, null)
#	var ret_ref = example.return_extended_ref()
#	assert_not_equal(ret_ref.get_instance_id(), 0)
#	assert_equal(ret_ref.get_id(), 0)
#	assert_equal(example.get_v4(), Vector4(1.2, 3.4, 5.6, 7.8))
#	assert_equal(example.test_node_argument(example), example)
#
#	# VarArg method calls.
#	var var_ref = ExampleRef.new()
#	assert_not_equal(example.extended_ref_checks(var_ref).get_instance_id(), var_ref.get_instance_id())
#	assert_equal(example.varargs_func("some", "arguments", "to", "test"), 4)
#	assert_equal(example.varargs_func_nv("some", "arguments", "to", "test"), 46)
#	example.varargs_func_void("some", "arguments", "to", "test")
#	assert_equal(custom_signal_emitted, ["varargs_func_void", 5])
#
#	# Method calls with default values.
#	assert_equal(example.def_args(), 300)
#	assert_equal(example.def_args(50), 250)
#	assert_equal(example.def_args(50, 100), 150)
#
#	# Array and Dictionary
#	assert_equal(example.test_array(), [1, 2])
#	assert_equal(example.test_tarray(), [ Vector2(1, 2), Vector2(2, 3) ])
#	assert_equal(example.test_dictionary(), {"hello": "world", "foo": "bar"})
#	var array: Array[int] = [1, 2, 3]
#	assert_equal(example.test_tarray_arg(array), 6)
#
#	# String += operator
#	assert_equal(example.test_string_ops(), "ABCĎE")
#
#	# UtilityFunctions::str()
#	assert_equal(example.test_str_utility(), "Hello, World! The answer is 42")
#
#	# Test converting string to char* and doing comparison.
#	assert_equal(example.test_string_is_fourty_two("blah"), false)
#	assert_equal(example.test_string_is_fourty_two("fourty two"), true)
#
#	# PackedArray iterators
#	assert_equal(example.test_vector_ops(), 105)
#
#	# Properties.
#	assert_equal(example.group_subgroup_custom_position, Vector2(0, 0))
#	example.group_subgroup_custom_position = Vector2(50, 50)
#	assert_equal(example.group_subgroup_custom_position, Vector2(50, 50))
#
#	# Constants.
#	assert_equal(Example.FIRST, 0)
#	assert_equal(Example.ANSWER_TO_EVERYTHING, 42)
#	assert_equal(Example.CONSTANT_WITHOUT_ENUM, 314)
#
#	# BitFields.
#	assert_equal(Example.FLAG_ONE, 1)
#	assert_equal(Example.FLAG_TWO, 2)
#	assert_equal(example.test_bitfield(0), 0)
#	assert_equal(example.test_bitfield(Example.FLAG_ONE | Example.FLAG_TWO), 3)
#
#	# RPCs.
#	assert_equal(example.return_last_rpc_arg(), 0)
#	example.test_rpc(42)
#	assert_equal(example.return_last_rpc_arg(), 42)
#	example.test_send_rpc(100)
#	assert_equal(example.return_last_rpc_arg(), 100)
#
#	# Virtual method.
#	var event = InputEventKey.new()
#	event.key_label = KEY_H
#	event.unicode = 72
#	get_viewport().push_input(event)
#	assert_equal(custom_signal_emitted, ["_input: H", 72])
#
#	exit_with_status()

func _on_Example_custom_signal(signal_name, value):
	custom_signal_emitted = [signal_name, value]
