#ifndef RX_DISPOSABLE_AUTODISPOSER_H
#define RX_DISPOSABLE_AUTODISPOSER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/disposable.h"
#include "exception/exception.h"

#include <memory>

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx::disposable {

class RxAutoDisposer : public RefCounted {
    GDCLASS(RxAutoDisposer, RefCounted)

private:
    std::shared_ptr<DisposableBase> _disp;

public:
    RxAutoDisposer() { throw NotImplementedException(); }
    RxAutoDisposer(const std::shared_ptr<DisposableBase>& disp) : _disp(disp) {}
    ~RxAutoDisposer() {}

protected:
    static inline void _bind_methods() {}

public:
    inline static void add_to(Object* obj, const std::shared_ptr<DisposableBase>& disp) {
        Ref<RxAutoDisposer> disposer(memnew(RxAutoDisposer(disp)));
        uint64_t id = reinterpret_cast<uint64_t>(disp.get());
        String meta_entry = "autodisposer" + String::num_uint64(id);
        obj->set_meta(meta_entry, disposer);
    }

    inline static void remove_from(Object* obj, const std::shared_ptr<DisposableBase>& disp) {
        uint64_t id = reinterpret_cast<uint64_t>(disp.get());
        String meta_entry = "autodisposer" + String::num_uint64(id);
        obj->remove_meta(meta_entry);
    }

    inline static void remove_from_and_dispose(Object* obj, const std::shared_ptr<DisposableBase>& disp) {
        remove_from(obj, disp);
        disp->dispose();
    }

};

}

#endif // RX_DISPOSABLE_AUTODISPOSER_H