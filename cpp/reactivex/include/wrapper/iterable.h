#ifndef RX_WRAPPER_ITERABLE_H
#define RX_WRAPPER_ITERABLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

#include "abstract/iterable.h"
#include "wrapper/iterator.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx::wrappers {

class RxIterable : public RefCounted {
    GDCLASS(RxIterable, RefCounted)

private:
    std::shared_ptr<IterableBase> _ptr;

public:
    RxIterable() { throw NotImplementedException(); }
    RxIterable(const std::shared_ptr<IterableBase> iterable) : _ptr(iterable) {}
    ~RxIterable(){}

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("iter"), &RxIterable::iter);
    }

public:
    static inline Ref<RxIterable> wrap(const std::shared_ptr<IterableBase>& iterable) {
        return memnew(RxIterable(iterable));
    }
    inline std::shared_ptr<IterableBase> unwrap() const { return this->_ptr; }

    Ref<RxIterator> iter();
};

};

#endif // RX_WRAPPER_ITERABLE_H