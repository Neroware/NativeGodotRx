#ifndef RX_ABSTRACT_DISPOSABLE_H
#define RX_ABSTRACT_DISPOSABLE_H

#include "typing.h"

namespace rx::abstract {

class DisposableBase {
public:
    virtual void dispose() = 0;
};

}

#endif // RX_ABSTRACT_DISPOSABLE_H