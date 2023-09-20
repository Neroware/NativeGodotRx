#ifndef RX_GODOTTYPES_H
#define RX_GODOTTYPES_H

#include "typing.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include <cassert>

using namespace godot;

namespace rx {

class RxNodeLifecycleListener : public Node {
    GDCLASS(RxNodeLifecycleListener, Node)

protected:
    inline static void _bind_methods() {
        ADD_SIGNAL(MethodInfo("_on_event", PropertyInfo(Variant::VARIANT_MAX, "value")));
    }
};

class RxListenerOnProcess : public RxNodeLifecycleListener {
    GDCLASS(RxListenerOnProcess, RxNodeLifecycleListener);

protected:
    inline static void _bind_methods() {}

public:
    inline void _process(double delta) {
        emit_signal("_on_event", delta);
    }
};

class RxListenerOnPhysicsProcess : public RxNodeLifecycleListener {
    GDCLASS(RxListenerOnPhysicsProcess, RxNodeLifecycleListener);

protected:
    inline static void _bind_methods() {}

public:
    inline void _physics_process(double delta) {
        emit_signal("_on_event", delta);
    }
};

class RxListenerOnInput : public RxNodeLifecycleListener {
    GDCLASS(RxListenerOnInput, RxNodeLifecycleListener);

protected:
    inline static void _bind_methods() {}

public:
    inline void _input(Ref<InputEvent> event) {
        emit_signal("_on_event", event);
    }
};

class RxListenerOnShortcutInput : public RxNodeLifecycleListener {
    GDCLASS(RxListenerOnShortcutInput, RxNodeLifecycleListener);

protected:
    inline static void _bind_methods() {}

public:
    inline void _shortcut_input(Ref<InputEvent> event) {
        emit_signal("_on_event", event);
    }
};

class RxListenerOnUnhandledInput : public RxNodeLifecycleListener {
    GDCLASS(RxListenerOnUnhandledInput, RxNodeLifecycleListener);

protected:
    inline static void _bind_methods() {}

public:
    inline void _unhandled_input(Ref<InputEvent> event) {
        emit_signal("_on_event", event);
    }
};

class RxListenerOnUnhandledKeyInput : public RxNodeLifecycleListener {
    GDCLASS(RxListenerOnUnhandledKeyInput, RxNodeLifecycleListener);

protected:
    inline static void _bind_methods() {}

public:
    inline void _unhandled_key_input(Ref<InputEvent> event) {
        emit_signal("_on_event", event);
    }
};

enum NodeLifecycleEvent {
    PROCESS,
    PHYSICS,
    INPUT,
    SHORTCUT_INPUT,
    UNHANDLED_INPUT,
    UNHANDLED_KEY_INPUT
};

static RxNodeLifecycleListener* create_lifecycle_listener(NodeLifecycleEvent event, const String& listener_name, int64_t process_priority, int64_t process_mode) {
    RxNodeLifecycleListener* res = nullptr;
    switch(event) {
        case PROCESS:
            res = memnew(RxListenerOnProcess); break;
        case PHYSICS:
            res = memnew(RxListenerOnPhysicsProcess); break;
        case INPUT:
            res = memnew(RxListenerOnInput); break;
        case SHORTCUT_INPUT:
            res = memnew(RxListenerOnShortcutInput); break;
        case UNHANDLED_INPUT:
            res = memnew(RxListenerOnUnhandledInput); break;
        case UNHANDLED_KEY_INPUT:
            res = memnew(RxListenerOnUnhandledKeyInput); break;
        default:
            assert(false && "should not happen!");
    }
    return res;
}

class RxHttpRequestResult : public RefCounted {
    GDCLASS(RxHttpRequestResult, RefCounted)

public:
    int result;
    int response_code;
    PackedStringArray headers;
    PackedByteArray body;
    String decoded;

protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("_get_result"), &RxHttpRequestResult::_get_result);
        ClassDB::bind_method(D_METHOD("_get_response_code"), &RxHttpRequestResult::_get_response_code);
        ClassDB::bind_method(D_METHOD("_get_headers"), &RxHttpRequestResult::_get_headers);
        ClassDB::bind_method(D_METHOD("_get_body"), &RxHttpRequestResult::_get_body);
        ClassDB::bind_method(D_METHOD("_get_decoded"), &RxHttpRequestResult::_get_decoded);

        ADD_PROPERTY(PropertyInfo(Variant::INT, "result"), "", "_get_result");
        ADD_PROPERTY(PropertyInfo(Variant::INT, "response_code"), "", "_get_response_code");
        ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "headers"), "", "_get_headers");
        ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "body"), "", "_get_body");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "decoded"), "", "_get_decoded");
    }

public:
    RxHttpRequestResult() { throw NotImplementedException(); }
    RxHttpRequestResult(int result_, int response_code_ = -1, const PackedStringArray& headers_ = PackedStringArray(), const PackedByteArray& body_ = PackedByteArray(), const String& decoded_ = "")
        : result(result_), response_code(response_code_), headers(headers_), body(body_), decoded(decoded_) {}
    ~RxHttpRequestResult(){}

    inline int _get_result() { return this->result; }
    inline int _get_response_code() { return this->response_code; }
    inline PackedStringArray _get_headers() { return this->headers; }
    inline PackedByteArray _get_body() { return this->body; }
    String _get_decoded() { return this->decoded; }

};

} // END namespace rx

#endif // RX_GODOTTYPES_H