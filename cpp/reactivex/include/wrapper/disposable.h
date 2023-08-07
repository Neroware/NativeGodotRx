#ifndef RX_WRAPPER_DISPOSABLE_H
#define RX_WRAPPER_DISPOSABLE_H

#include "wrapper/wrapper.h"

#include "abstract/disposable.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx {
namespace wrappers {

class RxDisposable : public RefCounted {
    GDCLASS(RxDisposable, RefCounted)
    RX_ABSTRACT_WRAPPER(RxDisposable, DisposableBase)
    
protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("dispose"), &RxDisposable::dispose);
        ClassDB::bind_method(D_METHOD("dispose_with", "obj"), &RxDisposable::dispose_with);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxDisposable::equals);
    }
public:
    void dispose();
    void dispose_with(Object* obj);
};

} // END namespace wrapper

static dispose_t dispose_cb(const Callable& cb) {
    return dispose_t([cb](){
        return cb.callv(Array());
    });
}

} // END namespace rx

#endif // RX_WRAPPER_DISPOSABLE_H