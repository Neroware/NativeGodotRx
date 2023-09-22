#ifndef RX_TEMPLATES_OBSERVABLE_H
#define RX_TEMPLATES_OBSERVABLE_H

#include "wrapper/templates.h"

#include "abstract/observable.h"
#include "observable/observable.h"

using namespace rx::wrappers;
using namespace rx::abstract;

namespace rx::templates {

class Observable_ : public Observable {

private:
    Ref<RxObservableTemplate_> self;
protected:
    Observable_(){}
public:
    ~Observable_(){}
    inline observable_ getptr() { return std::static_pointer_cast<Observable_>(Observable::getptr()); }
    inline static observable_ get() { return std::shared_ptr<Observable_>(new Observable_()); }
    inline void _template(Ref<RxObservableTemplate_> t) { this->self = t; }

    disposable_t _subscribe_core(const observer_t& observer, const scheduler_t& scheduler = nullptr) override {
        return RX_TEMPLATE_CALL(_subscribe_core, self, getptr(), observer, scheduler);
    }

    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Observable>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

class ObservableBase_ : public ObservableBase, public std::enable_shared_from_this<ObservableBase_> {

private:
    Ref<RxObservableTemplate_Base_> self;
protected:
    ObservableBase_(){}
public:
    ~ObservableBase_(){}
    inline observable_base_ getptr() { return this->shared_from_this(); }
    inline static observable_base_ get() { return std::shared_ptr<ObservableBase_>(new ObservableBase_()); }
    inline void _template(Ref<RxObservableTemplate_Base_> t) { this->self = t; }

    disposable_t subscribe(
        const on_next_t& on_next = DEFAULT_ON_NEXT,
        const on_error_t& on_error = DEFAULT_ON_ERROR,
        const on_completed_t& on_completed = DEFAULT_ON_COMPLETED,
        const scheduler_t& scheduler = nullptr
    ) override {
        return RX_TEMPLATE_CALL(_subscribe, self, getptr(), memnew(RxOnNext(on_next)), memnew(RxOnError(on_error)), memnew(RxOnCompleted(on_completed)), scheduler);
    }
    disposable_t subscribe(
        const observer_t& observer, 
        const scheduler_t& scheduler = nullptr
    ) override {
        return RX_TEMPLATE_CALL(_subscribe_observer, self, getptr(), observer, scheduler);
    }

    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous ObservableBase>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

} // END namespace rx::templates

#endif // RX_TEMPLATES_OBSERVABLE_H