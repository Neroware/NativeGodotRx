#ifndef RX_ABSTRACT_DISPOSABLE_H
#define RX_ABSTRACT_DISPOSABLE_H

#include <functional>

namespace rx {
namespace abstract {

class DisposableBase {
public:
    virtual void dispose() = 0;
};

}

typedef std::function<void()> dispose_t;

}

#endif // RX_ABSTRACT_DISPOSABLE_H