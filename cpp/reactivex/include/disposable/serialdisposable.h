#ifndef RX_DISPOSABLE_SERIALDISPOSABLE_H
#define RX_DISPOSABLE_SERIALDISPOSABLE_H

#include "abstract/disposable.h"
#include "internal/rlock.h"
#include "exception/exception.h"

#include <memory>

namespace rx::disposable {

class SerialDisposable : public DisposableBase {
    
public:
    std::shared_ptr<DisposableBase> current;
    bool is_disposed = false;
    RLock lock;

public:
    ~SerialDisposable() { this->dispose(); }
    void dispose() override;

    std::shared_ptr<DisposableBase> get_disposable();
    void set_disposable(const std::shared_ptr<DisposableBase>& value);

};

} // END namespace rx::disposable

#endif // RX_DISPOSABLE_SERIALDISPOSABLE_H