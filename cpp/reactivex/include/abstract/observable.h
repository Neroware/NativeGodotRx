#ifndef RX_ABSTRACT_OBSERVABLE_H
#define RX_ABSTRACT_OBSERVABLE_H

#include "basic.h"
#include "typing.h"

namespace rx::abstract {

class ObservableBase {

public:
    virtual disposable_t subscribe(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed, const scheduler_t& scheduler = nullptr) = 0;
    virtual disposable_t subscribe(const observer_t& observer, const scheduler_t& scheduler = nullptr) = 0;

};

} // END namespace rx::abstract

#endif // RX_ABSTRACT_OBSERVABLE_H