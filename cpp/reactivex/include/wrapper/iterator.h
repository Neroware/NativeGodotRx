#ifndef RX_WRAPPER_ITERATOR_H
#define RX_WRAPPER_ITERATOR_H

#include "wrapper/abstract.h"

#include "templates/iterator.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::templates;

namespace rx::wrappers {


/* ================================================================================ */
//                                   TEMPLATES
/* ================================================================================ */

class RxIterator_ : public RxIteratorBase {
    GDCLASS(RxIterator_, RxIteratorBase)
    _RX_WRAPPER(RxIterator_, Iterator_, RxIteratorBase, IteratorBase)

protected:
    static inline void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxIterator_)
        ClassDB::bind_method(D_METHOD("_template", "t"), &RxIterator_::_template);
    }

public:
    RxIterator_() 
        : RxIteratorBase(std::static_pointer_cast<IteratorBase>(Iterator_::get())), 
        _ptr(std::static_pointer_cast<Iterator_>(RxIteratorBase::getptr())) {}
    
    inline String _to_string() const {
        return "[" + this->_ptr->classname() + ":" + UtilityFunctions::str(
            reinterpret_cast<uint64_t>(this->_ptr.get())) + "]";
    }
    
    inline void _template(Ref<RxIteratorTemplate_> t) {
        this->_ptr->_template(t);
    }
    
}; // END class RxIterator_

} // END namespace rx::wrappers

#endif // RX_WRAPPER_ITERATOR_H