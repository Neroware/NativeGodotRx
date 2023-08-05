#ifndef RX_WRAPPER_ITERATOR_H
#define RX_WRAPPER_ITERATOR_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

#include "abstract/iterator.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx::wrappers {

class RxIterator : public RefCounted {
    GDCLASS(RxIterator, RefCounted)

private:
    std::shared_ptr<IteratorBase> _ptr;

public:
    RxIterator() { throw NotImplementedException(); }
    RxIterator(const std::shared_ptr<IteratorBase>& it) : _ptr(it) {}
    ~RxIterator(){}

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("next"), &RxIterator::next);
        ClassDB::bind_method(D_METHOD("has_next"), &RxIterator::has_next);
        ClassDB::bind_method(D_METHOD("end"), &RxIterator::end);
        ClassDB::bind_method(D_METHOD("foreach", "cb"), &RxIterator::foreach);
        ClassDB::bind_method(D_METHOD("enumerate", "cb"), &RxIterator::enumerate);
    }

public:
    static inline Ref<RxIterator> wrap(const std::shared_ptr<IteratorBase>& it) {
        return memnew(RxIterator(it));
    }
    inline std::shared_ptr<IteratorBase> unwrap() const { return this->_ptr; }

    Variant next();
    bool has_next();
    Variant end();

    void foreach(const Callable& what);
    void enumerate(const Callable& what);

};

};

#endif // RX_WRAPPER_ITERATOR_H