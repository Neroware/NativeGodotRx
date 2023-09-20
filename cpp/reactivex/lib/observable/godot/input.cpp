#include "observable/definitions.h"

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_joypad_button.hpp>
#include <godot_cpp/classes/input_event_midi.hpp>
#include <godot_cpp/classes/input_event_screen_touch.hpp>
#include <godot_cpp/classes/input_event_screen_drag.hpp>

namespace rx::observable {

rx_observable_t godot::on_mouse_down_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value) { return IS_CLASS(value, InputEventMouseButton); }),
        op::filter([](const Ref<InputEventMouseButton> ev) { return ev->is_pressed(); })
    );
}

rx_observable_t godot::on_mouse_up_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value) { return IS_CLASS(value, InputEventMouseButton); }),
        op::filter([](const Ref<InputEventMouseButton> ev) { return !ev->is_pressed(); })
    );
}

rx_observable_t godot::on_mouse_double_click_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value) { return IS_CLASS(value, InputEventMouseButton); }),
        op::filter([](const Ref<InputEventMouseButton> ev) { return ev->is_pressed() && ev->is_double_click(); })
    );
}

rx_observable_t godot::on_mouse_motion_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value){ return IS_CLASS(value, InputEventMouseMotion); })
    );
}

rx_observable_t godot::relative_mouse_movement_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value){ return IS_CLASS(value, InputEventMouseMotion); }),
        op::map([](Ref<InputEventMouseMotion> ev) { return ev->get_relative(); })
    );
}

rx_observable_t godot::on_key_just_pressed_(int key) {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value) { return IS_CLASS(value, InputEventKey); }),
        op::filter([key](const Ref<InputEventKey> ev) { return ev->get_keycode() == key && ev->is_pressed() && !ev->is_echo(); })
    );
}

rx_observable_t godot::on_key_pressed_(int key) {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value) { return IS_CLASS(value, InputEventKey); }),
        op::filter([key](const Ref<InputEventKey> ev) { return ev->get_keycode() == key && ev->is_pressed(); })
    );
}

rx_observable_t godot::on_key_just_released_(int key) {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value) { return IS_CLASS(value, InputEventKey); }),
        op::filter([key](const Ref<InputEventKey> ev) { return ev->get_keycode() == key && !ev->is_pressed(); })
    );
}

rx_observable_t godot::on_screen_touch_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value){ return IS_CLASS(value, InputEventScreenTouch); })
    );
}

rx_observable_t godot::on_screen_drag_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value){ return IS_CLASS(value, InputEventScreenDrag); })
    );
}

rx_observable_t godot::on_midi_event_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value){ return IS_CLASS(value, InputEventMIDI); })
    );
}

rx_observable_t godot::on_joypad_button_down_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value){ return IS_CLASS(value, InputEventJoypadButton); }),
        op::filter([](Ref<InputEventJoypadButton> ev) { return !ev->is_echo() && ev->is_pressed(); })
    );
}

rx_observable_t godot::on_joypad_button_pressed_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value){ return IS_CLASS(value, InputEventJoypadButton); }),
        op::filter([](Ref<InputEventJoypadButton> ev) { return ev->is_pressed(); })
    );
}

rx_observable_t godot::on_joypad_button_released_() {
    return on_input_as_observable(GDRX->get_root())->pipe(
        op::filter([](const Variant& value){ return IS_CLASS(value, InputEventJoypadButton); }),
        op::filter([](Ref<InputEventJoypadButton> ev) { return !ev->is_pressed(); })
    );
}

} // END namespace rx::observable