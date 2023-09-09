#ifndef RX_ABSTRACT_ITERABLE_H
#define RX_ABSTRACT_ITERABLE_H

#include "typing.h"

namespace rx::abstract {

class IterableBase {

public:
    virtual iterator_t iter() = 0;
    
};

}; // END namespace abstract

#endif // RX_ABSTRACT_ITERABLE_H