#ifndef RX_GODOTRX_H
#define RX_GODOTRX_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/scheduler.h"
#include "internal/time.h"

using namespace godot;

class __GDRxSingleton__ : public RefCounted {
    GDCLASS(__GDRxSingleton__, RefCounted);

private:

protected:

	static inline void _bind_methods() {

    }

public:

    __GDRxSingleton__(){}
    ~__GDRxSingleton__(){}

};

#endif // RX_GODOTRX_H