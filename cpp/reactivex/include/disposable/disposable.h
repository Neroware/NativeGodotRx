#ifndef RX_DISPOSABLE_DISPOSABLE_H
#define RX_DISPOSABLE_DISPOSABLE_H

#include "abstract/disposable.h"
#include "internal/rlock.h"
#include "exception/exception.h"
#include "basic.h"

namespace rx::disposable {

class Disposable : public DisposableBase {

public:
    bool is_disposed = false;
    dispose_t action;
    RLock lock;

public:
    Disposable(const dispose_t& action_ = rx::basic::noop<>)
        : action(action_) {}
    ~Disposable(){ this->dispose(); }

    void dispose() override;
};

} // END namespace disposable

#endif // RX_DISPOSABLE_DISPOSABLE_H