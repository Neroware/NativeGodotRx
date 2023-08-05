#include "wrapper/lock.h"

namespace rx::wrappers {

void RxLock::lock() { 
    this->_ptr->lock(); 
}

void RxLock::unlock() { 
    this->_ptr->unlock(); 
}

bool RxLock::try_lock() { 
    return this->_ptr->try_lock(); 
}

}