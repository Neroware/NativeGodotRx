#ifndef RX_WRAPPER_STARTABLE_H
#define RX_WRAPPER_STARTABLE_H

#include "wrapper/abstract.h"

#include "templates/startable.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::templates;

namespace rx::wrappers {


/* ================================================================================ */
//                                   TEMPLATES
/* ================================================================================ */

class RxStartable_ : public RxStartableBase {
    GDCLASS(RxStartable_, RxStartableBase)
    _RX_WRAPPER(RxStartable_, Startable_, RxStartableBase, StartableBase)

protected:
    static inline void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxStartable_)
        ClassDB::bind_method(D_METHOD("_template", "t"), &RxStartable_::_template);
    }

public:
    RxStartable_() 
        : RxStartableBase(std::static_pointer_cast<StartableBase>(Startable_::get())), 
        _ptr(std::static_pointer_cast<Startable_>(RxStartableBase::getptr())) {}
    
    inline String _to_string() const {
        return "[" + this->_ptr->classname() + ":" + UtilityFunctions::str(
            reinterpret_cast<uint64_t>(this->_ptr.get())) + "]";
    }
    
    inline void _template(Ref<RxStartableTemplate_> t) {
        this->_ptr->_template(t);
    }
    
}; // END class RxStartable_

} // END namespace rx::wrappers

#endif // RX_WRAPPER_STARTABLE_H