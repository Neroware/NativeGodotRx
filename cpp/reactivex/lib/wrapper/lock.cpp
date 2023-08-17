#include "wrapper/lock.h"

namespace rx::wrappers {

void RxLockBase::lock() { 
    this->_ptr->lock(); 
}

void RxLockBase::unlock() { 
    this->_ptr->unlock(); 
}

bool RxLockBase::try_lock() { 
    return this->_ptr->try_lock(); 
}

}