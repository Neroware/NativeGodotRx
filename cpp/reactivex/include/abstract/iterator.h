#ifndef RX_ABSTRACT_ITERATOR_H
#define RX_ABSTRACT_ITERATOR_H

#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace rx::abstract {

class IteratorBase {

public:
    virtual Variant next() = 0;
    virtual bool has_next() = 0;
    virtual Variant end() = 0;
    
};

}; // END namespace abstract

#endif // RX_ABSTRACT_ITERATOR_H