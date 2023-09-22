#ifndef RX_WRAPPER_ITERABLE_H
#define RX_WRAPPER_ITERABLE_H

#include "wrapper/abstract.h"

#include "templates/iterable.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::templates;

namespace rx::wrappers {


/* ================================================================================ */
//                                   TEMPLATES
/* ================================================================================ */

class RxIterable_ : public RxIterableBase {
    GDCLASS(RxIterable_, RxIterableBase)
    _RX_WRAPPER(RxIterable_, Iterable_, RxIterableBase, IterableBase)

protected:
    static inline void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxIterable_)
        ClassDB::bind_method(D_METHOD("_template", "t"), &RxIterable_::_template);
    }

public:
    RxIterable_() 
        : RxIterableBase(std::static_pointer_cast<IterableBase>(Iterable_::get())), 
        _ptr(std::static_pointer_cast<Iterable_>(RxIterableBase::getptr())) {}
    
    inline String _to_string() const {
        return "[" + this->_ptr->classname() + ":" + UtilityFunctions::str(
            reinterpret_cast<uint64_t>(this->_ptr.get())) + "]";
    }
    
    inline void _template(Ref<RxIterableTemplate_> t) {
        this->_ptr->_template(t);
    }
    
}; // END class RxIterable_

} // END namespace rx::wrappers

#endif // RX_WRAPPER_ITERABLE_H