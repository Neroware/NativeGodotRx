#ifndef RX_WRAPPER_ITERATOR_H
#define RX_WRAPPER_ITERATOR_H

#include "wrapper/wrapper.h"

#include "abstract/iterator.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx::wrappers {

class RxIterator : public RefCounted {
    GDCLASS(RxIterator, RefCounted)
    RX_ABSTRACT_WRAPPER(RxIterator, IteratorBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("next"), &RxIterator::next);
        ClassDB::bind_method(D_METHOD("has_next"), &RxIterator::has_next);
        ClassDB::bind_method(D_METHOD("foreach", "cb"), &RxIterator::foreach);
        ClassDB::bind_method(D_METHOD("enumerate", "cb"), &RxIterator::enumerate);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxIterator::equals);
    }

public:
    Variant next();
    bool has_next();

    void foreach(const Callable& what);
    void enumerate(const Callable& what);
};

};

#endif // RX_WRAPPER_ITERATOR_H