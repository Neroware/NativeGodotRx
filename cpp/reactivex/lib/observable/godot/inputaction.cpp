#include "observable/definitions.h"

#include <godot_cpp/classes/input.hpp>

namespace rx::observable {

enum InputState {
    RELEASED = 0, JUST_PRESSED = 1, PRESSED = 2, JUST_RELEASED = 3
};

rx_observable_t godot::from_input_action_(const String& input_action, const rx_observable_t& checks) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {

        auto prev_pressed = std::make_shared<bool>(false);
        auto curr_pressed = std::make_shared<bool>(false);

        on_next_t on_next = [=](const Variant&) {
            *prev_pressed = *curr_pressed;
            *curr_pressed = Input::get_singleton()->is_action_pressed(input_action);
            if (*prev_pressed != *curr_pressed) {
                observer->on_next(*curr_pressed ? InputState::JUST_PRESSED : InputState::JUST_RELEASED);
            }
            observer->on_next(*curr_pressed ? InputState::PRESSED : InputState::RELEASED);
        };

        return checks->subscribe(
            on_next, ONERROR_FWD(observer),
            ONCOMPLETED_FWD(observer), scheduler
        );
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable