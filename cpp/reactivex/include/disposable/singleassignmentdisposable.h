#ifndef RX_DISPOSABLE_SINGLEASSIGNMENTDISPOSABLE_H
#define RX_DISPOSABLE_SINGLEASSIGNMENTDISPOSABLE_H

#include "abstract/disposable.h"
#include "internal/rlock.h"
#include "exception/exception.h"

#include <memory>

namespace rx::disposable {

class SingleAssignmentDisposable : public DisposableBase {

public:
    disposable_t current;
    bool is_disposed = false;
    RLock lock;

public:
    ~SingleAssignmentDisposable() { this->dispose(); }
    void dispose() override;

    disposable_t get_disposable();
    void set_disposable(const disposable_t& value);

};

} // END namespace rx::disposable

#endif // RX_DISPOSABLE_SINGLEASSIGNMENTDISPOSABLE_H