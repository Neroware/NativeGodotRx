#ifndef RX_DISPOSABLE_COMPOSITEDISPOSABLE_H
#define RX_DISPOSABLE_COMPOSITEDISPOSABLE_H

#include "abstract/disposable.h"
#include "internal/rlock.h"
#include "exception/exception.h"

#include <memory>
#include <list>

namespace rx::disposable {

typedef std::shared_ptr<DisposableBase> disposable_t;
typedef std::list<disposable_t> disposable_list_t;

class CompositeDisposable : public DisposableBase {

public:
    disposable_list_t disposable;
    bool is_disposed = false;
    RLock lock;

public:
    CompositeDisposable(const disposable_list_t& items = {}) 
        : disposable(items) {}
    ~CompositeDisposable(){ this->dispose(); }

    void dispose() override;

    void add(const disposable_t& item);
    bool remove(const disposable_t& item);
    void clear();
    bool contains(const disposable_t& item) const;
    disposable_list_t to_list() const;
    int size() const;

};

} // END namespace rx::disposable

#endif // RX_DISPOSABLE_COMPOSITEDISPOSABLE_H