#ifndef RX_WRAPPER_OBSERVER_H
#define RX_WRAPPER_OBSERVER_H

#include "wrapper/wrapper.h"

#include "abstract/observer.h"
#include "exception/exception.h"
#include "exception/exceptionwrapper.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx {
namespace wrappers {

class RxObserver : public RefCounted {
    GDCLASS(RxObserver, RefCounted)
    RX_ABSTRACT_WRAPPER(RxObserver, ObserverBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("on_next", "item"), &RxObserver::on_next);
        ClassDB::bind_method(D_METHOD("on_error", "error"), &RxObserver::on_error);
        ClassDB::bind_method(D_METHOD("on_completed"), &RxObserver::on_completed);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxObserver::equals);
    }
public:
    void on_next(const Variant& item);
    void on_error(Ref<RxError> error);
    void on_completed();
};

} // END namespace wrapper

static on_next_t on_next_cb(const Callable& cb) {
    return on_next_t([cb](const Variant& item){
        return cb.callv(Array::make(item));
    });
}
static on_error_t on_error_cb(const Callable& cb) {
    return on_error_t([cb](const std::exception& e){
        return cb.callv(Array::make(rx::exception::RxError::wrap(e)));
    });
}
static on_completed_t on_completed_cb(const Callable& cb) {
    return on_completed_t([cb](){
        return cb.callv(Array());
    });
}

} // END namespace rx

#endif // RX_WRAPPER_OBSERVER_H