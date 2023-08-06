#ifndef RX_WRAPPER_OBSERVER_H
#define RX_WRAPPER_OBSERVER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/observer.h"

#include "exception/exception.h"
#include "exception/exceptionwrapper.h"

#include <memory>

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx {
namespace wrappers {

class RxObserver : public RefCounted {
    GDCLASS(RxObserver, RefCounted)

private:
    std::shared_ptr<ObserverBase> _ptr;

public:
    RxObserver() { throw NotImplementedException(); }
    RxObserver(const std::shared_ptr<ObserverBase>& disp) : _ptr(disp) {}
    ~RxObserver(){}

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("on_next", "item"), &RxObserver::on_next);
        ClassDB::bind_method(D_METHOD("on_error", "error"), &RxObserver::on_error);
        ClassDB::bind_method(D_METHOD("on_completed"), &RxObserver::on_completed);

        ClassDB::bind_method(D_METHOD("equals", "other"), &RxObserver::equals);
    }

public:
    static Ref<RxObserver> wrap(const std::shared_ptr<ObserverBase>& disp) {
        return memnew(RxObserver(disp));
    }
    std::shared_ptr<ObserverBase> unwrap() const { return this->_ptr; }

    void on_next(const Variant& item);
    void on_error(Ref<RxError> error);
    void on_completed();

    inline String _to_string() { return "[RxObserver:" + UtilityFunctions::str(reinterpret_cast<uint64_t>(this->_ptr.get())) + "]"; }
    inline bool equals(Ref<RxObserver> other) { return this->_ptr.get() == other->_ptr.get(); }

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