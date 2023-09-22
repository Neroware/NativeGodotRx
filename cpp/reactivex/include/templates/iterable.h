#ifndef RX_TEMPLATES_ITERABLE_H
#define RX_TEMPLATES_ITERABLE_H

#include "wrapper/templates.h"

#include "abstract/iterable.h"

using namespace rx::wrappers;
using namespace rx::abstract;

namespace rx::templates {

class Iterable_ : public IterableBase, public std::enable_shared_from_this<Iterable_> {

private:
    Ref<RxIterableTemplate_> self;
protected:
    Iterable_(){}
public:
    ~Iterable_(){}
    inline iterable_ getptr() { return this->shared_from_this(); }
    inline static iterable_ get() { return std::shared_ptr<Iterable_>(new Iterable_()); }
    inline void _template(Ref<RxIterableTemplate_> t) { this->self = t; }

    inline iterator_t iter() override {
        return RX_TEMPLATE_CALL(_iter, self, getptr());
    }

    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Iterable>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

} // END namespace rx::templates

#endif // RX_TEMPLATES_DISPOSABLE_H