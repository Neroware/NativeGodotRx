#ifndef RX_ABSTRACT_OBSERVER_H
#define RX_ABSTRACT_OBSERVER_H

#include <godot_cpp/variant/variant.hpp>
#include "exception/exception.h"

#include <functional>

using namespace godot;
using namespace rx::exception;

namespace rx {

typedef std::function<void(const Variant&)> on_next_t;
typedef std::function<void(const std::exception&)> on_error_t;
typedef std::function<void()> on_completed_t;

namespace abstract {

class ObserverBase {

public:
    virtual void on_next(const Variant& i) {
        throw NotImplementedException();
    }
    virtual void on_error(const std::exception& e) {
        throw NotImplementedException();
    }
    virtual void on_completed() {
        throw NotImplementedException();
    }
};

} // END namespace abstract
} // END namespace rx

#endif // RX_ABSTRACT_OBSERVER_H