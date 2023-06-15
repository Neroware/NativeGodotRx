/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/godot.hpp>

#include "internal/basic.h"

#include "exception/exceptionwrapper.h"

#include "abstract/lock.h"
#include "abstract/disposable.h"
#include "abstract/scheduler.h"
#include "abstract/periodicscheduler.h"
#include "abstract/observer.h"
#include "abstract/observable.h"
#include "abstract/subject.h"
#include "abstract/iterable.h"
#include "abstract/iterator.h"
#include "abstract/startable.h"

#include "internal/rlock.h"
#include "internal/lock.h"
#include "internal/time.h"
#include "internal/weakkeydictionary.h"
#include "internal/thread.h"
#include "internal/tuple.h"
#include "internal/iterator.h"
#include "internal/lambda.h"
#include "internal/concurrency.h"

#include "disposable/disposable.h"
#include "disposable/booleandisposable.h"
#include "disposable/compositedisposable.h"
#include "disposable/multiassignmentdisposable.h"
#include "disposable/refcountdisposable.h"
#include "disposable/singleassignmentdisposable.h"
#include "disposable/scheduleddisposable.h"
#include "disposable/serialdisposable.h"
#include "disposable/autodisposable.h"

#include "scheduler/scheduleditem.h"
#include "scheduler/scheduler.h"
#include "scheduler/periodicscheduler.h"
#include "scheduler/trampoline.h"
#include "scheduler/trampolinescheduler.h"
#include "scheduler/currentthreadscheduler.h"
#include "scheduler/immediatescheduler.h"
#include "scheduler/timeoutscheduler.h"
#include "scheduler/scenetreetimeoutscheduler.h"

#include "godotrx.h"

using namespace godot;

void initialize_rx_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	// exception
	ClassDB::register_abstract_class<RxError>();
	// abstract
	ClassDB::register_abstract_class<LockBase>();
	ClassDB::register_abstract_class<DisposableBase>();
	ClassDB::register_abstract_class<SchedulerBase>();
	ClassDB::register_abstract_class<PeriodicSchedulerBase>();
	ClassDB::register_abstract_class<ObservableBase>();
	ClassDB::register_abstract_class<ObserverBase>();
	ClassDB::register_abstract_class<SubjectAsObserver>();
	ClassDB::register_abstract_class<SubjectAsObservable>();
	ClassDB::register_abstract_class<IterableBase>();
	ClassDB::register_abstract_class<IteratorBase>();
	ClassDB::register_abstract_class<StartableBase>();
	// internal
	ClassDB::register_abstract_class<RLock>();
	ClassDB::register_abstract_class<Lock>();
	ClassDB::register_abstract_class<AbsoluteTime>();
	ClassDB::register_abstract_class<RelativeTime>();
	ClassDB::register_abstract_class<WeakKeyDictionary>();
	ClassDB::register_abstract_class<RxThread>();
	ClassDB::register_abstract_class<RxMainThread>();
	ClassDB::register_abstract_class<RxLambda>();
	ClassDB::register_abstract_class<Tuple>();
	ClassDB::register_abstract_class<Iterator>();
	ClassDB::register_abstract_class<ItEnd>();
	ClassDB::register_abstract_class<ArrayIterator>();
	ClassDB::register_abstract_class<DictionaryIterator>();
	ClassDB::register_abstract_class<StartableThread>();
	// disposable
	ClassDB::register_abstract_class<Disposable>();
	ClassDB::register_abstract_class<BooleanDisposable>();
	ClassDB::register_abstract_class<CompositeDisposable>();
	ClassDB::register_abstract_class<MultiAssignmentDisposable>();
	ClassDB::register_abstract_class<InnerDisposable>();
	ClassDB::register_abstract_class<RefCountDisposable>();
	ClassDB::register_abstract_class<SingleAssignmentDisposable>();
	ClassDB::register_abstract_class<ScheduledDisposable>();
	ClassDB::register_abstract_class<SerialDisposable>();
	ClassDB::register_abstract_class<AutoDisposable>();
	// scheduler
	ClassDB::register_abstract_class<ScheduledItem>();
	ClassDB::register_abstract_class<Scheduler>();
	ClassDB::register_abstract_class<PeriodicScheduler>();
	ClassDB::register_abstract_class<Trampoline>();
	ClassDB::register_abstract_class<TrampolineScheduler>();
	ClassDB::register_abstract_class<CurrentThreadScheduler>();
	ClassDB::register_abstract_class<_CurrentThreadScheduler_Local>();
	ClassDB::register_abstract_class<CurrentThreadSchedulerSingleton>();
	ClassDB::register_abstract_class<ImmediateScheduler>();
	ClassDB::register_abstract_class<TimeoutScheduler>();
	ClassDB::register_abstract_class<SceneTreeTimeoutScheduler>();

	ClassDB::register_abstract_class<__GDRxSingleton__>();

	Engine::get_singleton()->register_singleton(GDRX_SINGLETON_NAME, memnew(__GDRxSingleton__));
}

void uninitialize_rx_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT rx_library_init(const GDExtensionInterface *p_interface, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(initialize_rx_module);
	init_obj.register_terminator(uninitialize_rx_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
