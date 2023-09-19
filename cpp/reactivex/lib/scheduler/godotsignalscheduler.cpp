#include "scheduler/godotsignalscheduler.h"

#include "disposable/disposable.h"

#include "godotrx.h"

using namespace rx::disposable;

namespace rx::scheduler {

static std::pair<Signal, int> get_signal_from_owner(const Object* owner, const String& signal_name) {
    if (!owner->has_signal(signal_name)) {
        throw BadArgumentException("signal not found in owner");
    }
    Signal signal = owner->get(signal_name);
    assert(!signal.is_null());
    int n_args = -1;
	auto sig_lst = owner->get_signal_list();
	for (auto i = 0ul; i < sig_lst.size(); i++) {
        Dictionary dict = sig_lst[i];
        if (dict["name"].operator StringName() == signal.get_name()) {
            n_args = dict["args"].operator godot::Array().size();
        }
    }
    return std::make_pair(signal, n_args);
}

std::shared_ptr<GodotSignalScheduler> GodotSignalScheduler::singleton() {
    return GDRX->GodotSignalScheduler_;
}

disposable_t GodotSignalScheduler::schedule(const scheduled_action_t& action, const Variant& state) {
    return SceneTreeTimeoutScheduler::singleton()->schedule(action, state);
}

disposable_t GodotSignalScheduler::schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state) {
    auto _duetime = std::max(DELTA_ZERO, duetime);
    return this->schedule_absolute(this->now() + _duetime, action, state);
}

disposable_t GodotSignalScheduler::schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state) {
    return SceneTreeTimeoutScheduler::singleton()->schedule_absolute(duetime, action, state);
}

disposable_t GodotSignalScheduler::schedule_signal(const Object* owner, const StringName& signal_name, const periodic_action_t& action, const Variant& state) {
    auto signal_info = get_signal_from_owner(owner, signal_name);
    Signal signal = signal_info.first;
    int n_args = signal_info.second;
    if (n_args > 16) {
        throw BadArgumentException("GodotRx onwly supports up to 16 signal arguments!");
    }

    Ref<RefCounted> owner_ref = DYN_CAST_OR_NULL(Variant(owner), RefCounted);
    Ref<RxSignalCallback> callback = memnew(RxSignalCallback(action));
    Callable callback_func(*callback, "_callback" + UtilityFunctions::str(n_args));
    signal.connect(callback_func);

    dispose_t dispose = [signal, callback, callback_func, owner_ref, owner]() {
        if (owner_ref.is_null() && !UtilityFunctions::is_instance_valid(owner)) {
            return;
        }
        Signal _signal = signal; // Weird cancellation of const.
        _signal.disconnect(callback_func);
    };

    return std::make_shared<Disposable>(dispose);
}

} // END namespace rx::scheduler