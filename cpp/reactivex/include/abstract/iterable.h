#ifndef RX_ABSTRACT_ITERABLE_H
#define RX_ABSTRACT_ITERABLE_H

#include "abstract/iterator.h"

#include <memory>

namespace rx::abstract {

class IterableBase {

public:
    virtual std::shared_ptr<IteratorBase> iter() = 0;
    
};

}; // END namespace abstract

#endif // RX_ABSTRACT_ITERABLE_H