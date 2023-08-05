#ifndef RX_WRAPPER_ITERABLE_H
#define RX_WRAPPER_ITERABLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
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

        ClassDB::bind_method(D_METHOD("equals", "other"), &RxIterable::equals);
    }

public:
    static inline Ref<RxIterable> wrap(const std::shared_ptr<IterableBase>& iterable) {
        return memnew(RxIterable(iterable));
    }
    inline std::shared_ptr<IterableBase> unwrap() const { return this->_ptr; }

    Ref<RxIterator> iter();

    inline String _to_string() { return "[RxIterable:" + UtilityFunctions::str(reinterpret_cast<uint64_t>(this->_ptr.get())) + "]"; }
    inline bool equals(Ref<RxIterable> other) { return this->_ptr.get() == other->_ptr.get(); }
};

} // END namespace rx::wrappers

#endif // RX_WRAPPER_ITERABLE_H