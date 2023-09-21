// amb.h
static Ref<RxObservable> Amb(const Variant& sources);
// case.h
static Ref<RxObservable> Case(const Callable& mapper, const Dictionary& sources, Ref<RxObservable> default_source = VNULL);
// catch.h
static Ref<RxObservable> Catch(const Variant& sources);
// combinelatest.h
static Ref<RxObservable> CombineLatest(const Variant& sources);
// concat.h
static Ref<RxObservable> Concat(const Variant& sources);
// create.h
static Ref<RxObservable> Create(const Callable& subscribe = CBNULL);
// defer.h
static Ref<RxObservable> Defer(const Callable& factory);
// empty.h
static Ref<RxObservable> Empty(Ref<RxSchedulerBase> scheduler = VNULL);
// forkjoin.h
static Ref<RxObservable> ForkJoin(const Variant& sources);
// fromiterable.h
static Ref<RxObservable> FromIterable(const Variant& iterable, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> From(const Variant& iterable, Ref<RxSchedulerBase> scheduler = VNULL);
// generate.h
static Ref<RxObservable> Generate(const Variant& initial_state, const Callable& condition, const Callable& iterate);
// generatewithrelativetime.h
static Ref<RxObservable> GenerateWithRelativeTime(const Variant& initial_state, const Callable& condition, const Callable& iterate, const Callable& time_mapper);
// ifthen.h
static Ref<RxObservable> IfThen(const Callable& condition, Ref<RxObservable> then_source, Ref<RxObservable> else_source);
// interval.h
static Ref<RxObservable> Interval(const Variant& period, Ref<RxSchedulerBase> scheduler = VNULL);
// merge.h
static Ref<RxObservable> Merge(const Variant& sources);
// never.h
static Ref<RxObservable> Never();
// onerrorresumenext.h
static Ref<RxObservable> OnErrorResumeNext(const Variant& sources);
static Ref<RxObservable> OnErrorResumeFactory(const Variant& sources);
// range.h
static Ref<RxObservable> Range(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1);
// repeat.h
static Ref<RxObservable> Repeat(const Variant& value, int64_t repeat_count = -1);
// returnvalue.h
static Ref<RxObservable> ReturnValue(const Variant& value, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> Just(const Variant& value, Ref<RxSchedulerBase> scheduler = VNULL);
// throw.h
static Ref<RxObservable> ThrowMessage(const String& what, const String& type = "RxError", Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> ThrowError(Ref<RxError> error, Ref<RxSchedulerBase> scheduler = VNULL);
// timer.h
static Ref<RxObservable> TimerAbs(Ref<AbsoluteTime> duetime, Ref<RelativeTime> period = VNULL, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> TimerRel(Ref<RelativeTime> duetime, Ref<RelativeTime> period = VNULL, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> Timer(double duetime, double period = -1.0, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> PeriodicTimerRel(Ref<RelativeTime> period, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> PeriodicTimer(double period, Ref<RxSchedulerBase> scheduler = VNULL);
// using.h
static Ref<RxObservable> Using(const Callable& resource_factory, const Callable& observable_factory);
// withlatestfrom.h
static Ref<RxObservable> WithLatestFrom(Ref<RxObservable> parent, const Variant& sources);
// zip.h
static Ref<RxObservable> Zip(const Variant& sources);


/* OPERATORS */

// _all.h
Ref<RxObservable> all(const Callable& predicate);
// _amb.h
Ref<RxObservable> amb(Ref<RxObservable> right_source);
// _asobservable.h
Ref<RxObservable> as_observable();
// _average.h
Ref<RxObservable> average(const Callable& key_mapper = CBNULL);
// _buffer.h
Ref<RxObservable> buffer(Ref<RxObservable> boundaries);
Ref<RxObservable> buffer_when(const Callable& closing_mapper);
Ref<RxObservable> buffer_toggle(Ref<RxObservable> openings, const Callable& closing_mapper);
Ref<RxObservable> buffer_with_count(int64_t count, int64_t skip = -1);
// _bufferwithtime.h
Ref<RxObservable> buffer_with_time(const Variant& timespan, const Variant& timeshift = DTZERO, Ref<RxSchedulerBase> scheduler = VNULL);
// _bufferwithtimeorcount.h
Ref<RxObservable> buffer_with_time_or_count(const Variant& timespan, int64_t count, Ref<RxSchedulerBase> scheduler = VNULL);
// _catch.h
Ref<RxObservable> catch_with_handler(const Callable& handler);
Ref<RxObservable> catch_with(Ref<RxObservable> handler);
// _combine_latest.h
Ref<RxObservable> combine_latestv(const Variant& sources);
Ref<RxObservable> combine_latest(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _concat.h
Ref<RxObservable> concatv(const Variant& sources);
Ref<RxObservable> concat(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _contains.h
Ref<RxObservable> contains(const Variant& value, const Callable& comparer = CBNULL);
// _count.h
Ref<RxObservable> count(const Callable& predicate = CBNULL);
// _debounce.h
Ref<RxObservable> debounce(const Variant& duetime, Ref<RxSchedulerBase> scheduler = VNULL);
Ref<RxObservable> throttle_with_mapper(const Callable& throttle_duration_mapper);
// _defaultifempty.h
Ref<RxObservable> default_if_empty(const Variant& default_value = VNULL);
// _delay.h
Ref<RxObservable> delay(const Variant& duetime, Ref<RxSchedulerBase> scheduler = VNULL);
// _delaysubscription.h
Ref<RxObservable> delay_subscription(const Variant& duetime, Ref<RxSchedulerBase> scheduler = VNULL);
// _delaywithmapper.h
Ref<RxObservable> delay_with_mapper(Ref<RxObservable> subscription_delay = VNULL, const Callable& subscription_delay_mapper = CBNULL, Ref<RxObservable> delay_duration_mapper = VNULL);
// _dematerialize.h
Ref<RxObservable> dematerialize();
// _distinct.h
Ref<RxObservable> distinct(const Callable& key_mapper = CBNULL, const Callable& comparer = CBNULL);
// _distinctuntilchanged.h
Ref<RxObservable> distinct_until_changed(const Callable& key_mapper = CBNULL, const Callable& comparer = CBNULL);
// _do.h
Ref<RxObservable> do_action(const Callable& on_next = CBNULL, const Callable& on_error = CBNULL, const Callable& on_completed = CBNULL);
Ref<RxObservable> do_observer(Ref<RxObserverBase> observer);
Ref<RxObservable> do_after_next(const Callable& after_next);
Ref<RxObservable> do_on_subscribe(const Callable& on_subscribe);
Ref<RxObservable> do_on_dispose(const Callable& on_dispose);
Ref<RxObservable> do_on_terminate(const Callable& on_terminate);
Ref<RxObservable> do_finally(const Callable& finally_action);
// _whiledo.h
Ref<RxObservable> do_while(const Callable& condition);
// _elementatordefault.h
Ref<RxObservable> element_at_or_default(uint64_t index, bool has_default = false, const Variant& default_value = VNULL);
// _exclusive.h
Ref<RxObservable> exclusive();
// _expand.h
Ref<RxObservable> expand(const Callable& mapper);
// _filter.h
Ref<RxObservable> filter(const Callable& predicate);
Ref<RxObservable> filter_indexed(const Callable& predicate = CBNULL);
// _finallyaction.h
Ref<RxObservable> finally_action(const Callable& action);
// _find.h
Ref<RxObservable> find_value(const Callable& predicate, bool yield_index);
// _first.h
Ref<RxObservable> first(const Callable& predicate = CBNULL);
// _firstordefault.h
Ref<RxObservable> first_or_default(const Callable& predicate = CBNULL, const Variant& default_value = VNULL);
// _flatmap.h
Ref<RxObservable> flat_map(const Callable& mapper = CBNULL);
Ref<RxObservable> flat_map_indexed(const Callable& mapper = CBNULL);
Ref<RxObservable> flat_map_latest(const Callable& mapper = CBNULL);
// _forkjoin.h
Ref<RxObservable> fork_joinv(const Variant& sources);
Ref<RxObservable> fork_join(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _groupby.h
Ref<RxObservable> group_by(const Callable& key_mapper, const Callable& element_mapper = CBNULL, const Callable& subject_mapper = CBNULL);
// _groupbyuntil.h
Ref<RxObservable> group_by_until(const Callable& key_mapper, const Callable& duration_mapper, const Callable& element_mapper = CBNULL, const Callable& subject_mapper = CBNULL);
// _groupjoin.h
Ref<RxObservable> group_join(Ref<RxObservable> right, const Callable& left_duration_mapper, const Callable& right_duration_mapper);
// _ignoreelements.h
Ref<RxObservable> ignore_elements();
// _isempty.h
Ref<RxObservable> is_empty();
// _join.h
Ref<RxObservable> join(Ref<RxObservable> right, const Callable& left_duration_mapper, const Callable& right_duration_mapper);
// _last.h
Ref<RxObservable> last(const Callable& predicate = CBNULL);
// _lastordefault.h
Ref<RxObservable> last_or_default(const Variant& default_value = VNULL, const Callable& predicate = CBNULL);
// _map.h
Ref<RxObservable> map(const Callable& mapper = CBNULL);
Ref<RxObservable> map_indexed(const Callable& mapper = CBNULL);
// _materialize.h
Ref<RxObservable> materialize();
// _max.h
Ref<RxObservable> max(const Callable& comparer = CBNULL);
// _maxby.h
Ref<RxObservable> max_by(const Callable& key_mapper, const Callable& comparer = CBNULL);
// _merge.h
Ref<RxObservable> merge_all();
Ref<RxObservable> mergev(const Variant& sources, int64_t max_concurrent = -1);
Ref<RxObservable> merge(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _min.h
Ref<RxObservable> min(const Callable& comparer = CBNULL);
// _minby.h
Ref<RxObservable> min_by(const Callable& key_mapper, const Callable& comparer = CBNULL);
// _multicast.h
Ref<RxConnectableObservable> multicast(Ref<RxSubject> subject);
Ref<RxObservable> multicast_factory(const Callable& subject_factory, const Callable& mapper = CBNULL);
// _observeon.h
Ref<RxObservable> observe_on(Ref<RxSchedulerBase> scheduler);
// _onerrorresumenext.h
Ref<RxObservable> on_error_resume_next(Ref<RxObservable> second);
// _pairwise.h
Ref<RxObservable> pairwise();
// _partition.h
TypedArray<RxObservable> partition(const Callable& predicate);
TypedArray<RxObservable> partition_indexed(const Callable& predicate);
// _pluck.h
Ref<RxObservable> pluck(const Variant& key);
Ref<RxObservable> pluck_attr(const String& key);
// _publish.h
Ref<RxConnectableObservable> publish();
Ref<RxObservable> publish_mapped(const Callable& mapper);
Ref<RxObservable> share();
// _publishvalue.h
Ref<RxObservable> publish_value(const Variant& initial_value, const Callable& mapper = CBNULL);
// _reduce.h
Ref<RxObservable> reduce(const Callable& accumulator, const Variant& seed = UNSET);
// _repeat.h
Ref<RxObservable> repeat(int64_t repeat_count = -1);
// _replay.h
Ref<RxObservable> replay(const Callable& mapper = CBNULL, uint64_t buffer_size = 1024ul, const Variant& window = DTZERO, Ref<RxSchedulerBase> scheduler = VNULL);
// _retry.h
Ref<RxObservable> retry(int64_t retry_count = -1);
// _sample.h
Ref<RxObservable> sample_with_time(const Variant& sampler, Ref<RxSchedulerBase> scheduler = VNULL);
Ref<RxObservable> sample(Ref<RxObservable> sampler);
// _scan.h
Ref<RxObservable> scan(const Callable& accumulator, const Variant& seed = UNSET);
// _sequenceequal.h
Ref<RxObservable> sequence_equal(Ref<RxObservable> second, const Callable& comparer = CBNULL);
Ref<RxObservable>  sequence_equal_iterable(const Variant& iterable, const Callable& comparer = CBNULL);
// _single.h
Ref<RxObservable> single(const Callable& predicate = CBNULL);
// _singleordefault.h
Ref<RxObservable> single_or_default_async(bool has_default = false, const Variant& default_value = VNULL);
Ref<RxObservable> single_or_default(const Callable& predicate, const Variant& default_value = VNULL);
// _skip.h
Ref<RxObservable> skip(uint64_t count);
// _skiplast.h
Ref<RxObservable> skip_last(uint64_t count);
// _skiplastwithtime.h
Ref<RxObservable> skip_last_with_time_rel(Ref<RelativeTime> duration, Ref<RxSchedulerBase> scheduler = VNULL);
Ref<RxObservable> skip_last_with_time_abs(Ref<AbsoluteTime> duration, Ref<RxSchedulerBase> scheduler = VNULL);
Ref<RxObservable> skip_last_with_time(double duration, Ref<RxSchedulerBase> scheduler = VNULL);
// _skipuntil.h
Ref<RxObservable> skip_until(Ref<RxObservable> other);
// _skipuntilwithtime.h
Ref<RxObservable> skip_until_with_time_rel(Ref<RelativeTime> start_time, Ref<RxSchedulerBase> scheduler = VNULL);
Ref<RxObservable> skip_until_with_time_abs(Ref<AbsoluteTime> start_time, Ref<RxSchedulerBase> scheduler = VNULL);
Ref<RxObservable> skip_until_with_time(double start_time, Ref<RxSchedulerBase> scheduler = VNULL);
// _skipwhile.h
Ref<RxObservable> skip_while(const Callable& predicate);
Ref<RxObservable> skip_while_indexed(const Callable& predicate);
// _skipwithtime.h
Ref<RxObservable> skip_with_time(const Variant& duration, Ref<RxSchedulerBase> scheduler = VNULL);
// _slice.h
Ref<RxObservable> slice(int64_t start = 0l, int64_t stop = INT64_MAX, int64_t step = 1l);
// _some.h
Ref<RxObservable> some(const Callable& predicate = CBNULL);
// _startwith.h
Ref<RxObservable> start_with(const Variant& args);
// _subscribeon.h
Ref<RxObservable> subscribe_on(Ref<RxSchedulerBase> scheduler);
// _sum.h
Ref<RxObservable> sum(const Callable& key_mapper = CBNULL);
// _switchlatest.h
Ref<RxObservable> switch_latest();
// _take.h
Ref<RxObservable> take(uint64_t count);
// _takelast.h
Ref<RxObservable> take_last(uint64_t count);
// _takelastwithbuffer.h
Ref<RxObservable> take_last_buffer(uint64_t count);
// _takelastwithtime.h
Ref<RxObservable> take_last_with_time(const Variant& duration, Ref<RxSchedulerBase> scheduler = VNULL);
// _takeuntil.h
Ref<RxObservable> take_until(Ref<RxObservable> other);
// _takeuntilwithtime.h
Ref<RxObservable> take_until_with_time(const Variant& end_time, Ref<RxSchedulerBase> scheduler = VNULL);
// _takewhile.h
Ref<RxObservable> take_while(const Callable& predciate, bool inclusive = false);
Ref<RxObservable> take_while_indexed(const Callable& predciate, bool inclusive = false);
// _takewithtime.h
Ref<RxObservable> take_with_time(const Variant& duration, Ref<RxSchedulerBase> scheduler = VNULL);
// _throttlefirst.h
Ref<RxObservable> throttle_first(const Variant& window_duration, Ref<RxSchedulerBase> scheduler = VNULL);
// _timeinterval.h
Ref<RxObservable> time_interval(Ref<RxSchedulerBase> scheduler = VNULL);
// _timeout.h
Ref<RxObservable> timeout(const Variant& duetime, Ref<RxObservable> other = VNULL, Ref<RxSchedulerBase> scheduler = VNULL);
// _timeoutwithmapper.h
Ref<RxObservable> timeout_with_mapper(Ref<RxObservable> first_timeout = VNULL, const Callable& timeout_duration_mapper = CBNULL, Ref<RxObservable> other = VNULL);
// _timestamp.h
Ref<RxObservable> timestamp(Ref<RxSchedulerBase> scheduler = VNULL);
// _todict.h
Ref<RxObservable> to_dict(const Callable& key_mapper, const Callable& element_mapper = CBNULL);
// _tolist.h
Ref<RxObservable> to_list();
Ref<RxObservable> to_iterable();
// _toset.h
Ref<RxObservable> to_set(const Callable& comparer = CBNULL);
// _whiledo.h
Ref<RxObservable> while_do(const Callable& condition);
// _window.h
Ref<RxObservable> window_toggle(Ref<RxObservable> openings, const Callable& closing_mapper);
Ref<RxObservable> window(Ref<RxObservable> boundaries);
Ref<RxObservable> window_when(const Callable& closing_mapper);
// _windowwithcount.h
Ref<RxObservable> window_with_count(int64_t count, int64_t skip = -1);
// _windowwithtime.h
Ref<RxObservable> window_with_time(const Variant& timespan, const Variant& timeshift = DTZERO, Ref<RxSchedulerBase> scheduler = VNULL);
// _windowwithtimeorcount.h
Ref<RxObservable> window_with_time_or_count(const Variant& timespan, int64_t count, Ref<RxSchedulerBase> scheduler = VNULL);
// _withlatestfrom.h
Ref<RxObservable> with_latest_fromv(const Variant& sources);
Ref<RxObservable> with_latest_from(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _zip.h
Ref<RxObservable> zipv(const Variant& sources);
Ref<RxObservable> zip(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
Ref<RxObservable> zip_with_iterable(const Variant& iterable);

/* CONNECTABLE */
static Ref<RxObservable> add_ref(Ref<RxObservable> xs, Ref<RxRefCountDisposable> r);

/* GODOT-specifics */
// fromsignal.h
static Ref<RxObservable> FromSignal(Object* owner, const StringName& signal_name, bool track_owner = false, Ref<RxSchedulerBase> scheduler = VNULL);
// input.h
static Ref<RxObservable> OnMouseDown();
static Ref<RxObservable> OnMouseUp();
static Ref<RxObservable> OnMouseDoubleClick();
static Ref<RxObservable> OnMouseMotion();
static Ref<RxObservable> RelativeMouseMovement();
static Ref<RxObservable> OnKeyJustPressed(int key);
static Ref<RxObservable> OnKeyPressed(int key);
static Ref<RxObservable> OnKeyJustReleased(int key);
static Ref<RxObservable> OnScreenTouch();
static Ref<RxObservable> OnScreenDrag();
static Ref<RxObservable> OnMidiEvent();
static Ref<RxObservable> OnJoypadButtonDown();
static Ref<RxObservable> OnJoypadButtonPressed();
static Ref<RxObservable> OnJoypadButtonReleased();
// inputaction.h
static Ref<RxObservable> FromInputAction(const String& input_action, Ref<RxObservable> checks);
// nodelifecycle.h
static Ref<RxObservable> OnProcessAsObservable(Node* conn);
static Ref<RxObservable> OnPhysicsAsObservable(Node* conn);
static Ref<RxObservable> OnInputAsObservable(Node* conn);
static Ref<RxObservable> OnShortcutInputAsObservable(Node* conn);
static Ref<RxObservable> OnUnhandledInputAsObservable(Node* conn);
static Ref<RxObservable> OnUnhandledKeyInputAsObservable(Node* conn);
// computeshader.h
static Ref<RxObservable> FromComputeShader(const String& shader_path, RenderingDevice* rd, const Vector3i& work_groups, const Array& uniform_sets = Array(), Ref<RxSchedulerBase> scheduler = VNULL);
// framevents.h
static Ref<RxObservable> OnIdleFrame();
static Ref<RxObservable> OnPhysicsStep();
static Ref<RxObservable> OnTreeChanged();
static Ref<RxObservable> OnFramePostDraw();
static Ref<RxObservable> OnFramePreDraw();
// scenetree.h
static Ref<RxObservable> OnTreeEnterAsObservable(Node* node);
static Ref<RxObservable> OnTreeExitAsObservable(Node* node);
static Ref<RxObservable> OnTreeExitingAsObservable(Node* node);
// httprequest.h
static Ref<RxObservable> FromHTTPRequest(const String& url, const Variant& request_data = "", bool raw = false, const String& encoding = "", HTTPRequest* requester = nullptr, const PackedStringArray& custom_headers = PackedStringArray(), bool tls_validate_domain = true, HTTPClient::Method method = HTTPClient::Method::METHOD_GET);
// _processtimeinterval.h
Ref<RxObservable> process_time_interval(double initial_time = 0.0);
Ref<RxObservable> physics_time_interval(double initial_time = 0.0);