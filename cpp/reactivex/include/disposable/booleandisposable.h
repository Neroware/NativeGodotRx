#ifndef RX_DISPOSABLE_BOOLEANDISPOSABLE_H
#define RX_DISPOSABLE_BOOLEANDISPOSABLE_H

#include "abstract/disposable.h"
#include "internal/rlock.h"

using namespace rx::abstract;

namespace rx::disposable {

class BooleanDisposable : public DisposableBase {

public:
    bool is_disposed = false;
    RLock lock;

public:
    ~BooleanDisposable() { this->dispose(); }
    void dispose() override;
};

} // END namespace rx::disposable

#endif // RX_DISPOSABLE_BOOLEANDISPOSABLE_H