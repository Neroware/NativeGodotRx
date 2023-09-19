#ifndef RX_ABSTRACT_GODOTSIGNALSCHEDULER_H
#define RX_ABSTRACT_GODOTSIGNALSCHEDULER_H

#include "typing.h"

#include <godot_cpp/variant/signal.hpp>

using namespace godot;

namespace rx::abstract {

class GodotSignalSchedulerBase {

public:
    virtual disposable_t schedule_signal(const Object* owner, const StringName& signal_name, const periodic_action_t& action, const Variant& state = Variant()) = 0;
};

} // END namespace rx::abstract

#endif // RX_ABSTRACT_GODOTSIGNALSCHEDULER_H