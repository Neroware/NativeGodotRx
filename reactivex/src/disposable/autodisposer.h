#ifndef RX_AUTODISPOSER_H
#define RX_AUTODISPOSER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/disposable.h"

using namespace godot;

struct AutoDisposer {

    inline static void add_to(Object* obj, Ref<DisposableBase> disp) {
        uint64_t id = disp->get_instance_id();
        String meta_entry = "autodisposer" + String::num_uint64(id);
        obj->set_meta(meta_entry, disp);
    }

    inline static void remove_from(Object* obj, Ref<DisposableBase> disp) {
        uint64_t id = disp->get_instance_id();
        String meta_entry = "autodisposer" + String::num_uint64(id);
        obj->remove_meta(meta_entry);
    }

    inline static void remove_from_and_dispose(Object* obj, Ref<DisposableBase> disp) {
        remove_from(obj, disp);
        disp->dispose();
    }

};

#endif // RX_AUTODISPOSER_H