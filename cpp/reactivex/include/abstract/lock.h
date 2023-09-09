#ifndef RX_ABSTRACT_LOCK_H
#define RX_ABSTRACT_LOCK_H

#include "typing.h"

namespace rx::abstract {

class LockBase {
public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual bool try_lock() = 0;
};

};

#endif // RX_ABSTRACT_LOCK_H