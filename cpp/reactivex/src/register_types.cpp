/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "example.h"
#include "tests.h"

#include "godotrx.h"
// exception
#include "exception/exceptionwrapper.h"
// internal
#include "internal/time.h"
#include "internal/thread.h"
#include "internal/utils.h"
// wrappers
#include "wrapper/abstract.h"
/* #include "wrapper/scheduler.h"
#include "wrapper/disposable.h"
#include "wrapper/observer.h"
#include "wrapper/observable.h"
#include "wrapper/subject.h" */
#include "wrapper/notificationwrapper.h"
// disposable
#include "disposable/autodisposer.h"
// scheduler
// #include "scheduler/scenetreetimeoutscheduler.h"

using namespace godot;

void initialize_reactivex_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_class<ExampleRef>();
	ClassDB::register_class<ExampleMin>();
	ClassDB::register_class<Example>();
	ClassDB::register_class<ExampleVirtual>(true);
	ClassDB::register_abstract_class<ExampleAbstract>();

	// exception
	ClassDB::register_abstract_class<rx::exception::RxError>();
	// internal
	ClassDB::register_abstract_class<rx::AbsoluteTime>();
	ClassDB::register_abstract_class<rx::RelativeTime>();
	ClassDB::register_class<rx::ItEnd>();
	ClassDB::register_class<rx::RxThread>();
	ClassDB::register_class<rx::RxMainThread>();
	ClassDB::register_class<rx::NotSet>();
	// abstract
	ClassDB::register_abstract_class<rx::wrappers::RxLockBase>();
	ClassDB::register_abstract_class<rx::wrappers::RxDisposableBase>();
	ClassDB::register_abstract_class<rx::wrappers::RxIteratorBase>();
	ClassDB::register_abstract_class<rx::wrappers::RxIterableBase>();
	ClassDB::register_abstract_class<rx::wrappers::RxSchedulerBase>();
	ClassDB::register_abstract_class<rx::wrappers::RxPeriodicSchedulerBase>();
	ClassDB::register_abstract_class<rx::wrappers::RxStartableBase>();
	// ClassDB::register_abstract_class<rx::wrappers::RxScheduler>();
	ClassDB::register_abstract_class<rx::wrappers::RxObserverBase>();
	ClassDB::register_abstract_class<rx::wrappers::RxObservableBase>();
	// ClassDB::register_abstract_class<rx::wrappers::RxObservable>();
	ClassDB::register_abstract_class<rx::wrappers::RxSubjectBase>();
	ClassDB::register_abstract_class<rx::wrappers::RxNotification>();
	// disposable
	ClassDB::register_abstract_class<rx::disposable::RxAutoDisposer>();
	/* ClassDB::register_abstract_class<rx::wrappers::RxBooleanDisposable>();
	ClassDB::register_abstract_class<rx::wrappers::RxSingleAssignmentDisposable>();
	ClassDB::register_abstract_class<rx::wrappers::RxMultiAssignmentDisposable>();
	ClassDB::register_abstract_class<rx::wrappers::RxSerialDisposable>();
	ClassDB::register_abstract_class<rx::wrappers::RxDisposable>();
	ClassDB::register_abstract_class<rx::wrappers::RxRefCountDisposable>();
	ClassDB::register_abstract_class<rx::wrappers::RxScheduledDisposable>();
	ClassDB::register_abstract_class<rx::wrappers::RxCompositeDisposable>(); */
	// scheduler
	/* ClassDB::register_abstract_class<rx::scheduler::RxSceneTreeTimeout>();
	ClassDB::register_abstract_class<rx::wrappers::RxCatchScheduler>();
	ClassDB::register_abstract_class<rx::wrappers::RxTrampolineScheduler>();
	ClassDB::register_abstract_class<rx::wrappers::RxCurrentThreadScheduler>();
	ClassDB::register_abstract_class<rx::wrappers::RxEventLoopScheduler>();
	ClassDB::register_abstract_class<rx::wrappers::RxImmediateScheduler>();
	ClassDB::register_abstract_class<rx::wrappers::RxNewThreadScheduler>();
	ClassDB::register_abstract_class<rx::wrappers::RxSceneTreeTimeoutScheduler>();
	ClassDB::register_abstract_class<rx::wrappers::RxTimeoutScheduler>(); */
	// observable
	/* ClassDB::register_abstract_class<rx::wrappers::RxConnectableObservable>();
	ClassDB::register_abstract_class<rx::wrappers::RxGroupedObservable>(); */
	// observer
	/* ClassDB::register_abstract_class<rx::wrappers::RxObserver>();
	ClassDB::register_abstract_class<rx::wrappers::RxScheduledObserver>();
	ClassDB::register_abstract_class<rx::wrappers::RxObserveOnObserver>();
	ClassDB::register_abstract_class<rx::wrappers::RxAutoDetachObserver>(); */
	// subject
	/* ClassDB::register_abstract_class<rx::wrappers::RxSubject>();
	ClassDB::register_abstract_class<rx::wrappers::RxReplaySubject>();
	ClassDB::register_abstract_class<rx::wrappers::RxBehaviorSubject>();
	ClassDB::register_abstract_class<rx::wrappers::RxAsyncSubject>(); */
	// notification
	ClassDB::register_abstract_class<rx::wrappers::RxNotificationOnNext>();
	ClassDB::register_abstract_class<rx::wrappers::RxNotificationOnError>();
	ClassDB::register_abstract_class<rx::wrappers::RxNotificationOnCompleted>();
	// GodotRx Singleton
	ClassDB::register_abstract_class<rx::__GDRxSingleton__>();

	Engine::get_singleton()->register_singleton(GDRX_SINGLETON_NAME, memnew(rx::__GDRxSingleton__));
}

void uninitialize_reactivex_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT reactivex_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_reactivex_module);
	init_obj.register_terminator(uninitialize_reactivex_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}