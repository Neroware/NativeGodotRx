#ifndef RX_LOCK_H
#define RX_LOCK_H

#include <condition_variable>
#include <mutex>

#include "abstract/lock.h"

using namespace rx::abstract;

namespace rx {

class Lock : public LockBase {

private:
    bool flag = false;
    std::mutex m;
    std::condition_variable cv;

public:
    void lock() override;
    void unlock() override;
    bool try_lock() override;
};

} // END namespace Lock

#endif // RX_LOCK_H