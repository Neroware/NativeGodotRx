/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#ifndef RX_REGISTER_TYPES_H
#define RX_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_reactivex_module(ModuleInitializationLevel p_level);
void uninitialize_reactivex_module(ModuleInitializationLevel p_level);

#endif // RX_REGISTER_TYPES_H
