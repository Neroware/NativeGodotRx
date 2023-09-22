#ifndef RX_TEMPLATES_ITERATOR_H
#define RX_TEMPLATES_ITERATOR_H

#include "wrapper/templates.h"

#include "abstract/iterator.h"

using namespace rx::wrappers;
using namespace rx::abstract;

namespace rx::templates {

class Iterator_ : public IteratorBase, public std::enable_shared_from_this<Iterator_> {

private:
    Ref<RxIteratorTemplate_> self;
protected:
    Iterator_(){}
public:
    ~Iterator_(){}
    inline iterator_ getptr() { return this->shared_from_this(); }
    inline static iterator_ get() { return std::shared_ptr<Iterator_>(new Iterator_()); }
    inline void _template(Ref<RxIteratorTemplate_> t) { this->self = t; }

    inline Variant next() override {
        return RX_TEMPLATE_CALL(_next, self, getptr());
    }
    inline bool has_next() override {
        return RX_TEMPLATE_CALL(_has_next, self, getptr());
    }

    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Iterator>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

} // END namespace rx::templates

#endif // RX_TEMPLATES_DISPOSABLE_H