#ifndef RX_ABSTRACT_STARTABLE_H
#define RX_ABSTRACT_STARTABLE_H

#include "typing.h"

namespace rx {
namespace abstract {

class StartableBase {

public:
    virtual void start() = 0;
    /* Needed for additional cleanup */
    virtual void await() = 0;
};

} // END namespace abstract

} // END namespace rx

#endif // RX_ABSTRACT_STARTABLE_H