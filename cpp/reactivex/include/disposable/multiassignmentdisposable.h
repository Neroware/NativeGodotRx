#ifndef RX_DISPOSABLE_MULTIASSIGNMENTDISPOSABLE_H
#define RX_DISPOSABLE_MULTIASSIGNMENTDISPOSABLE_H

#include "abstract/disposable.h"
#include "internal/rlock.h"
#include "exception/exception.h"

#include <memory>

namespace rx::disposable {

class MultiAssignmentDisposable : public DisposableBase {

public:
    std::shared_ptr<DisposableBase> current;
    bool is_disposed = false;
    RLock lock;

public:
    ~MultiAssignmentDisposable() { this->dispose(); }
    void dispose() override;

    std::shared_ptr<DisposableBase> get_disposable();
    void set_disposable(const std::shared_ptr<DisposableBase>& value);

};

} // END namespace rx::disposable

#endif // RX_DISPOSABLE_MULTIASSIGNMENTDISPOSABLE_H