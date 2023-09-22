#ifndef RX_TEMPLATES_OBSERVER_H
#define RX_TEMPLATES_OBSERVER_H

#include "wrapper/templates.h"

#include "abstract/observer.h"

using namespace rx::wrappers;
using namespace rx::abstract;

namespace rx::templates {

class Observer_ : public ObserverBase, public std::enable_shared_from_this<Observer_> {

private:
    Ref<RxObserverTemplate_> self;
protected:
    Observer_(){}
public:
    ~Observer_(){}
    inline observer_ getptr() { return this->shared_from_this(); }
    inline static observer_ get() { return std::shared_ptr<Observer_>(new Observer_()); }
    inline void _template(Ref<RxObserverTemplate_> t) { this->self = t; }

    void on_next(const Variant& i) override {
        RX_TEMPLATE_CALL(_on_next, self, getptr(), i);
    }
    void on_error(const error_t& e) override {
        RX_TEMPLATE_CALL(_on_error, self, getptr(), RxError::wrap(e));
    }
    void on_completed() override {
        RX_TEMPLATE_CALL(_on_completed, self, getptr());
    }

    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Observer>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

} // END namespace rx::templates

#endif // RX_TEMPLATES_OBSERVER_H