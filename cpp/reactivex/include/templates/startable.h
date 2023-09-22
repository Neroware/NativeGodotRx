#ifndef RX_TEMPLATES_STARTABLE_H
#define RX_TEMPLATES_STARTABLE_H

#include "wrapper/templates.h"

#include "abstract/startable.h"

using namespace rx::wrappers;
using namespace rx::abstract;

namespace rx::templates {

class Startable_ : public StartableBase, public std::enable_shared_from_this<Startable_> {

private:
    Ref<RxStartableTemplate_> self;
protected:
    Startable_(){}
public:
    ~Startable_(){}
    inline startable_ getptr() { return this->shared_from_this(); }
    inline static startable_ get() { return std::shared_ptr<Startable_>(new Startable_()); }
    inline void _template(Ref<RxStartableTemplate_> t) { this->self = t; }

    inline void start() override {
        RX_TEMPLATE_CALL(_start, self, getptr());
    }
    inline void await() override {
        RX_TEMPLATE_CALL(_await, self, getptr());
    }

    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Startable>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

} // END namespace rx::templates

#endif // RX_TEMPLATES_STARTABLE_H