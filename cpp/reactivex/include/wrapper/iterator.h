#ifndef RX_WRAPPER_ITERATOR_H
#define RX_WRAPPER_ITERATOR_H

#include "wrapper/wrapper.h"

#include "abstract/iterator.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx::wrappers {

class RxIteratorBase : public RefCounted {
    GDCLASS(RxIteratorBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxIteratorBase, IteratorBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("next"), &RxIteratorBase::next);
        ClassDB::bind_method(D_METHOD("has_next"), &RxIteratorBase::has_next);
        ClassDB::bind_method(D_METHOD("foreach", "cb"), &RxIteratorBase::foreach);
        ClassDB::bind_method(D_METHOD("enumerate", "cb"), &RxIteratorBase::enumerate);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxIteratorBase::equals);
    }

public:
    Variant next();
    bool has_next();

    void foreach(const Callable& what);
    void enumerate(const Callable& what);
};

};

#endif // RX_WRAPPER_ITERATOR_H