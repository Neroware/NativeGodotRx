#ifndef RX_ABSTRACT_SUBJECT_H
#define RX_ABSTRACT_SUBJECT_H

#include "abstract/observer.h"
#include "abstract/observable.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::exception;

namespace rx::abstract {

class SubjectBase : public ObservableBase, public ObserverBase {

public:
    virtual std::shared_ptr<DisposableBase> subscribe(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed, const std::shared_ptr<SchedulerBase>& scheduler) override {
        throw NotImplementedException();
    }
    virtual std::shared_ptr<DisposableBase> subscribe(const std::shared_ptr<ObserverBase>& observer, const std::shared_ptr<SchedulerBase>& scheduler) override {
        throw NotImplementedException();
    }

    virtual void on_next(const Variant& i) override {
        throw NotImplementedException();
    }
    virtual void on_error(const std::exception& e) override {
        throw NotImplementedException();
    }
    virtual void on_completed() override {
        throw NotImplementedException();
    }
};

};

#endif // RX_ABSTRACT_STARTABLE_H