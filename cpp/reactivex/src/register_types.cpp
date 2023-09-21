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
#include "internal/godot.h"
// wrappers
#include "wrapper/abstract.h"
#include "wrapper/lock.h"
#include "wrapper/scheduler.h"
#include "wrapper/disposable.h"
#include "wrapper/observer.h"
#include "wrapper/observable.h"
#include "wrapper/subject.h"
#include "wrapper/notificationwrapper.h"
// disposable
#include "disposable/autodisposer.h"
// scheduler
#include "scheduler/scenetreetimeoutscheduler.h"

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
	ClassDB::register_class<rx::AbsoluteTime>();
	ClassDB::register_class<rx::RelativeTime>();
	ClassDB::register_class<rx::RxTimeStamp>(true);
	ClassDB::register_class<rx::RxTimeInterval>(true);
	ClassDB::register_class<rx::ItEnd>();
	ClassDB::register_class<rx::RxThread>();
	ClassDB::register_class<rx::RxMainThread>();
	ClassDB::register_class<rx::NotSet>();
	ClassDB::register_class<rx::RxNodeLifecycleListener>();
	ClassDB::register_class<rx::RxListenerOnProcess>();
	ClassDB::register_class<rx::RxListenerOnPhysicsProcess>();
	ClassDB::register_class<rx::RxListenerOnInput>();
	ClassDB::register_class<rx::RxListenerOnShortcutInput>();
	ClassDB::register_class<rx::RxListenerOnUnhandledInput>();
	ClassDB::register_class<rx::RxListenerOnUnhandledKeyInput>();
	ClassDB::register_class<rx::RxHttpRequestResult>(true);
	// abstract
	ClassDB::register_class<rx::wrappers::RxLockBase>(true);
	ClassDB::register_class<rx::wrappers::RxDisposableBase>(true);
	ClassDB::register_class<rx::wrappers::RxIteratorBase>(true);
	ClassDB::register_class<rx::wrappers::RxIterableBase>(true);
	ClassDB::register_class<rx::wrappers::RxSchedulerBase>(true);
	ClassDB::register_class<rx::wrappers::RxPeriodicSchedulerBase>(true);
	ClassDB::register_class<rx::wrappers::RxStartableBase>(true);
	ClassDB::register_class<rx::wrappers::RxScheduler>(true);
	ClassDB::register_class<rx::wrappers::RxObserverBase>(true);
	ClassDB::register_class<rx::wrappers::RxObservableBase>(true);
	ClassDB::register_class<rx::wrappers::RxObservable>(true);
	ClassDB::register_class<rx::wrappers::RxSubjectBase>(true);
	ClassDB::register_class<rx::wrappers::RxNotification>(true);
	ClassDB::register_class<rx::wrappers::RxGodotSignalSchedulerBase>(true);
	// lock
	ClassDB::register_class<rx::wrappers::RxRecursiveLock>(true);
	ClassDB::register_class<rx::wrappers::RxLock>(true);
	// disposable
	ClassDB::register_class<rx::disposable::RxAutoDisposer>();
	ClassDB::register_class<rx::wrappers::RxBooleanDisposable>(true);
	ClassDB::register_class<rx::wrappers::RxSingleAssignmentDisposable>(true);
	ClassDB::register_class<rx::wrappers::RxMultiAssignmentDisposable>(true);
	ClassDB::register_class<rx::wrappers::RxSerialDisposable>(true);
	ClassDB::register_class<rx::wrappers::RxDisposable>(true);
	ClassDB::register_class<rx::wrappers::RxRefCountDisposable>(true);
	ClassDB::register_class<rx::wrappers::RxScheduledDisposable>(true);
	ClassDB::register_class<rx::wrappers::RxCompositeDisposable>(true);
	// scheduler
	ClassDB::register_class<rx::scheduler::RxSceneTreeTimeout>();
	ClassDB::register_class<rx::scheduler::RxSignalCallback>();
	ClassDB::register_class<rx::wrappers::RxCatchScheduler>(true);
	ClassDB::register_class<rx::wrappers::RxTrampolineScheduler>(true);
	ClassDB::register_class<rx::wrappers::RxCurrentThreadScheduler>(true);
	ClassDB::register_class<rx::wrappers::RxEventLoopScheduler>(true);
	ClassDB::register_class<rx::wrappers::RxImmediateScheduler>(true);
	ClassDB::register_class<rx::wrappers::RxNewThreadScheduler>(true);
	ClassDB::register_class<rx::wrappers::RxSceneTreeTimeoutScheduler>(true);
	ClassDB::register_class<rx::wrappers::RxTimeoutScheduler>(true);
	ClassDB::register_class<rx::wrappers::RxGodotSignalScheduler>(true);
	// observable
	ClassDB::register_class<rx::wrappers::RxConnectableObservable>(true);
	ClassDB::register_class<rx::wrappers::RxGroupedObservable>(true);
	ClassDB::register_class<rx::wrappers::RxReactiveProperty>(true);
	// observer
	ClassDB::register_class<rx::wrappers::RxObserver>(true);
	ClassDB::register_class<rx::wrappers::RxScheduledObserver>(true);
	ClassDB::register_class<rx::wrappers::RxObserveOnObserver>(true);
	ClassDB::register_class<rx::wrappers::RxAutoDetachObserver>(true);
	// subject
	ClassDB::register_class<rx::wrappers::RxSubject>(true);
	ClassDB::register_class<rx::wrappers::RxReplaySubject>(true);
	ClassDB::register_class<rx::wrappers::RxBehaviorSubject>(true);
	ClassDB::register_class<rx::wrappers::RxAsyncSubject>(true);
	// notification
	ClassDB::register_class<rx::wrappers::RxNotificationOnNext>(true);
	ClassDB::register_class<rx::wrappers::RxNotificationOnError>(true);
	ClassDB::register_class<rx::wrappers::RxNotificationOnCompleted>(true);
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