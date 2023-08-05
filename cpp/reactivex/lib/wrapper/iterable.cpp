#include "wrapper/iterable.h"

namespace rx::wrappers {

Ref<RxIterator> RxIterable::iter() { 
    return RxIterator::wrap(this->_ptr->iter()); 
}

}