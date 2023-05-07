#ifndef RX_ABSTRACT_SUBJECT_H
#define RX_ABSTRACT_SUBJECT_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/observer.h"
#include "abstract/observable.h"
#include "exception/exception.h"

using namespace godot;

class SubjectBase : public RefCounted {
    GDCLASS(SubjectBase, RefCounted);

public:
    SubjectBase(){}
    ~SubjectBase(){}

protected:
	static void _bind_methods() {
        BIND_VIRTUAL_METHOD(SubjectBase, subscribe);
        BIND_VIRTUAL_METHOD(SubjectBase, on_next);
        BIND_VIRTUAL_METHOD(SubjectBase, on_error);
        BIND_VIRTUAL_METHOD(SubjectBase, on_completed);
        BIND_VIRTUAL_METHOD(SubjectBase, as_observer);
        BIND_VIRTUAL_METHOD(SubjectBase, as_observable);
    }

public:
    virtual Ref<DisposableBase> subscribe(const Callable& on_next, const Callable& on_error, const Callable& on_completed, Ref<SchedulerBase> scheduler) {
        throw NotImplementedException();
    }
    virtual void on_next(const Variant& i) {
        throw NotImplementedException();
    }
    virtual void on_error(Ref<RxError> e) {
        throw NotImplementedException();
    }
    virtual void on_completed() {
        throw NotImplementedException();
    }
    virtual Ref<ObserverBase> as_observer() {
        throw NotImplementedException();
    }
    virtual Ref<ObservableBase> as_observable() {
        throw NotImplementedException();
    }
};

class SubjectAsObserver : public ObserverBase {
    GDCLASS(SubjectAsObserver, ObserverBase);

private:
    Ref<SubjectBase> subject;

public:
    SubjectAsObserver(Ref<SubjectBase> subject) : subject(subject) {}
    SubjectAsObserver(){}
    ~SubjectAsObserver(){}

protected:
	static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("on_next", "item"), &SubjectAsObserver::on_next);
        ClassDB::bind_method(D_METHOD("on_error", "err"), &SubjectAsObserver::on_error);
        ClassDB::bind_method(D_METHOD("on_completed"), &SubjectAsObserver::on_completed);
    }

public:
    void on_next(const Variant& i) override {
        subject->on_next(i);
    }
    void on_error(Ref<RxError> e) override {
        subject->on_error(e);
    }
    void on_completed() override {
        subject->on_completed();
    }

};

class SubjectAsObservable : public ObservableBase {
    GDCLASS(SubjectAsObservable, ObservableBase);

private:
    Ref<SubjectBase> subject;

public:
    SubjectAsObservable(Ref<SubjectBase> subject) : subject(subject) {}
    SubjectAsObservable(){}
    ~SubjectAsObservable(){}

protected:
	static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("subscribe", "on_next", "on_error", "on_completed", "scheduler"), &SubjectAsObservable::subscribe);    
    }

public:
    Ref<DisposableBase> subscribe(const Callable& on_next, const Callable& on_error, const Callable& on_completed, Ref<SchedulerBase> scheduler) override {
        return subject->subscribe(on_next, on_error, on_completed, scheduler);
    }

};

#endif // RX_ABSTRACT_STARTABLE_H