#ifndef RX_RLOCK_H
#define RX_RLOCK_H

#include <mutex>
#include <memory>

#include "abstract/lock.h"

using namespace rx::abstract;

namespace rx {

class RLock : public LockBase {

private:
    std::recursive_mutex rm;

public:
    void lock() override;
    void unlock() override;
    bool try_lock() override;
};

}

#endif // RX_RLOCK_H