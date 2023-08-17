#include "wrapper/iterator.h"

namespace rx::wrappers {

Variant RxIteratorBase::next() {
    return this->_ptr->next();
}
bool RxIteratorBase::has_next() {
    return this->_ptr->has_next();
}

void RxIteratorBase::foreach(const Callable& what) {
    while (this->has_next()) {
        Variant res = what.callv(Array::make(this->next()));
        if (res.get_type() == Variant::Type::BOOL && !res.booleanize())
            break;
    }
}
void RxIteratorBase::enumerate(const Callable& what) {
    uint64_t i = 0;
    while (this->has_next()) {
        Variant res = what.callv(Array::make(i++, this->next()));
        if (res.get_type() == Variant::Type::BOOL && !res.booleanize())
            break;
    }
}

} // END namespace rx::wrappers