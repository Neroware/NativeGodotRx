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

class AutoDisposer : public RefCounted {
    GDCLASS(AutoDisposer, RefCounted)

private:
    std::shared_ptr<DisposableBase> _disp;

public:
    AutoDisposer() { throw NotImplementedException(); }
    AutoDisposer(const std::shared_ptr<DisposableBase>& disp) : _disp(disp) {}
    ~AutoDisposer() {}

protected:
    static inline void _bind_methods() {}

public:
    inline static void add_to(Object* obj, const std::shared_ptr<DisposableBase>& disp) {
        Ref<AutoDisposer> disposer(memnew(AutoDisposer(disp)));
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