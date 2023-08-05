#ifndef RX_WRAPPER_DISPOSABLE_H
#define RX_WRAPPER_DISPOSABLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

#include "abstract/disposable.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx {
namespace wrappers {

class RxDisposable : public RefCounted {
    GDCLASS(RxDisposable, RefCounted)

private:
    std::shared_ptr<DisposableBase> _ptr;

public:
    RxDisposable() { throw NotImplementedException(); }
    RxDisposable(const std::shared_ptr<DisposableBase>& disp) : _ptr(disp) {}
    ~RxDisposable(){}

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("dispose"), &RxDisposable::dispose);
        ClassDB::bind_method(D_METHOD("dispose_with", "obj"), &RxDisposable::dispose_with);

        ClassDB::bind_method(D_METHOD("equals", "other"), &RxDisposable::equals);
    }

public:
    static Ref<RxDisposable> wrap(const std::shared_ptr<DisposableBase>& disp) {
        return memnew(RxDisposable(disp));
    }
    std::shared_ptr<DisposableBase> unwrap() const { return this->_ptr; }

    void dispose();
    void dispose_with(Object* obj);

    inline String _to_string() { return "[RxDisposable:" + UtilityFunctions::str(reinterpret_cast<uint64_t>(this->_ptr.get())) + "]"; }
    inline bool equals(Ref<RxDisposable> other) { return this->_ptr.get() == other->_ptr.get(); }

};

} // END namespace wrapper

static dispose_t dispose_cb(const Callable& cb) {
    return dispose_t([cb](){
        return cb.callv(Array());
    });
}

} // END namespace rx

#endif // RX_WRAPPER_DISPOSABLE_H