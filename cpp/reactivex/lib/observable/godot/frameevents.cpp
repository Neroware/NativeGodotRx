#include "observable/definitions.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

namespace rx::observable {

rx_observable_t godot::on_idle_frame_() {
    return from_signal(GDRX->get_tree(), "process_frame")->pipe(
        op::map([](const Variant&) { return GDRX->get_tree()->get_root()->get_process_delta_time(); })
    );
}

rx_observable_t godot::on_physics_step_() {
    return from_signal(GDRX->get_tree(), "physics_frame")->pipe(
        op::map([](const Variant&) { return GDRX->get_tree()->get_root()->get_physics_process_delta_time(); })
    );
}

rx_observable_t godot::on_tree_changed_() {
    return from_signal(GDRX->get_tree(), "tree_changed");
}

rx_observable_t godot::on_frame_post_draw_() {
    return from_signal(RenderingServer::get_singleton(), "frame_post_draw");
}

rx_observable_t godot::on_frame_pre_draw_() {
    return from_signal(RenderingServer::get_singleton(), "frame_pre_draw");
}


} // ENd namespace rx::observable