#include "observable/definitions.h"

#include "disposable/compositedisposable.h"
#include "disposable/disposable.h"

#include "internal/godot.h"

namespace rx::observable {

static rx_observable_t _node_lifecycle_event_as_observable(Node* conn, NodeLifecycleEvent event) {
    auto listener = std::make_shared<RxNodeLifecycleListener*>();
    auto count = std::make_shared<uint64_t>(0);

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {

        if (*count == 0) {
            String name = "GodotRx$Listener" + 
                UtilityFunctions::str(event) + 
                ":" + 
                UtilityFunctions::str(conn->get_instance_id());
            *listener = create_lifecycle_listener(
                event,
                name,
                conn->get_process_priority(),
                conn->get_process_mode()
            );
            conn->call_deferred("add_child", *listener);
        }
        ++(*count);

        dispose_t dispose = [=]() {
            --(*count);
            if (*count == 0 && UtilityFunctions::is_instance_valid(*listener)) {
                (*listener)->queue_free();
                *listener = nullptr;
            }
        };

        auto subscription = from_signal(*listener, "_on_event")->subscribe(
            ONNEXT_FWD(observer), ONERROR_FWD(observer),
            DEFAULT_ON_COMPLETED, scheduler_
        );


        return std::make_shared<CompositeDisposable>(
            subscription,
            std::make_shared<Disposable>(dispose)
        );
    };

    return Observable::get(subscribe);
}

rx_observable_t godot::on_process_as_observable_(Node* conn) {
    return _node_lifecycle_event_as_observable(conn, NodeLifecycleEvent::PROCESS);
}
rx_observable_t godot::on_physics_process_as_observable_(Node* conn) {
    return _node_lifecycle_event_as_observable(conn, NodeLifecycleEvent::PHYSICS);
}
rx_observable_t godot::on_input_as_observable_(Node* conn) {
    return _node_lifecycle_event_as_observable(conn, NodeLifecycleEvent::INPUT);
}
rx_observable_t godot::on_shortcut_input_as_observable_(Node* conn) {
    return _node_lifecycle_event_as_observable(conn, NodeLifecycleEvent::SHORTCUT_INPUT);
}
rx_observable_t godot::on_unhandled_input_as_observable_(Node* conn) {
    return _node_lifecycle_event_as_observable(conn, NodeLifecycleEvent::UNHANDLED_INPUT);
}
rx_observable_t godot::on_unhandled_key_input_as_observable_(Node* conn) {
    return _node_lifecycle_event_as_observable(conn, NodeLifecycleEvent::UNHANDLED_KEY_INPUT);
}

// rx_observable_t godot::on_process_as_observable_()

} // END namespace rx::observable