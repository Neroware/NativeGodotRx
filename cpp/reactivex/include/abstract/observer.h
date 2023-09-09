#ifndef RX_ABSTRACT_OBSERVER_H
#define RX_ABSTRACT_OBSERVER_H

#include "typing.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::exception;

namespace rx::abstract {

class ObserverBase {

public:
    virtual void on_next(const Variant& i) {
        throw NotImplementedException();
    }
    virtual void on_error(const error_t& e) {
        throw NotImplementedException();
    }
    virtual void on_completed() {
        throw NotImplementedException();
    }
};

} // END namespace rx::abstract

#endif // RX_ABSTRACT_OBSERVER_H