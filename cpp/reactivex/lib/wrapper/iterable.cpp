#include "wrapper/iterable.h"

namespace rx::wrappers {

Ref<RxIteratorBase> RxIterableBase::iter() { 
    return RxIteratorBase::wrap(this->_ptr->iter()); 
}

}