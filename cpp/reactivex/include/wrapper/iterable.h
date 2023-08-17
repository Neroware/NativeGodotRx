#ifndef RX_WRAPPER_ITERABLE_H
#define RX_WRAPPER_ITERABLE_H

#include "wrapper/wrapper.h"

#include "abstract/iterable.h"
#include "wrapper/iterator.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx::wrappers {

class RxIterableBase : public RefCounted {
    GDCLASS(RxIterableBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxIterableBase, IterableBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("iter"), &RxIterableBase::iter);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxIterableBase::equals);
    }
public:
    Ref<RxIteratorBase> iter();
};

} // END namespace rx::wrappers

#endif // RX_WRAPPER_ITERABLE_H