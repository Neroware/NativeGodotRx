#ifndef RX_DISPOSABLE_COMPOSITEDISPOSABLE_H
#define RX_DISPOSABLE_COMPOSITEDISPOSABLE_H

#include "abstract/disposable.h"
#include "abstract/iterable.h"
#include "abstract/iterator.h"

#include "internal/rlock.h"
#include "exception/exception.h"

namespace rx::disposable {

class CompositeDisposable : public DisposableBase {

public:
    disposable_list_t disposable;
    bool is_disposed = false;
    RLock lock;

public:
    CompositeDisposable() {}
    CompositeDisposable(const disposable_list_t& items) 
        : disposable(items) {}
    CompositeDisposable(const iterable_t& items) {
        iterator_t it = items->iter();
        while(it->has_next()) {
            disposable.push_back(it->next());
        }
    }
    template<typename ContainerT>
    CompositeDisposable(const ContainerT& items) 
        : disposable(items.begin(), items.end()) {}
    template<typename... Args>
    CompositeDisposable(const Args&... items) {
        this->disposable = disposable_list_t{items...};
    }
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