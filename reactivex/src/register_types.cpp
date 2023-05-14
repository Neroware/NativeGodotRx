/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "exception/exceptionwrapper.h"
#include "internal/basic.h"

#include "abstract/lock.h"
#include "abstract/disposable.h"
#include "abstract/scheduler.h"
#include "abstract/periodicscheduler.h"
#include "abstract/observer.h"
#include "abstract/observable.h"
#include "abstract/subject.h"

#include "internal/rlock.h"
#include "internal/lock.h"
#include "internal/time.h"

#include "disposable/disposable.h"
#include "disposable/booleandisposable.h"
#include "disposable/compositedisposable.h"
#include "disposable/multiassignmentdisposable.h"
#include "disposable/refcountdisposable.h"
#include "disposable/singleassignmentdisposable.h"
#include "disposable/scheduleddisposable.h"
#include "disposable/serialdisposable.h"

#include "scheduler/scheduler.h"
#include "scheduler/trampoline.h"

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
	// internal
	ClassDB::register_abstract_class<RLock>();
	ClassDB::register_abstract_class<Lock>();
	ClassDB::register_abstract_class<AbsoluteTime>();
	ClassDB::register_abstract_class<RelativeTime>();
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
	// scheduler
	ClassDB::register_abstract_class<Scheduler>();
	ClassDB::register_abstract_class<Trampoline>();
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
