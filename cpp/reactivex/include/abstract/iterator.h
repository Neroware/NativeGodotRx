#ifndef RX_ABSTRACT_ITERATOR_H
#define RX_ABSTRACT_ITERATOR_H

#include "typing.h"

using namespace godot;

namespace rx::abstract {

class IteratorBase {

public:
    virtual Variant next() = 0;
    virtual bool has_next() = 0;
};

}; // END namespace rx::abstract

#endif // RX_ABSTRACT_ITERATOR_H