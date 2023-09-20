#include "observable/definitions.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

namespace rx::observable {

rx_observable_t godot::on_tree_enter_as_observable_(Node* node) {
    return from_signal(node, "tree_entered");
}

rx_observable_t godot::on_tree_exit_as_observable_(Node* node) {
    return from_signal(node, "tree_exited");
}

rx_observable_t godot::on_tree_exiting_as_observable_(Node* node) {
    return from_signal(node, "tree_exiting");
}

} // ENd namespace rx::observable