#ifndef RX_DISPOSABLE_REFCOUNTDISPOSABLE_H
#define RX_DISPOSABLE_REFCOUNTDISPOSABLE_H

#include "abstract/disposable.h"
#include "basic.h"
#include "internal/rlock.h"
#include "exception/exception.h"

#include <memory>

namespace rx::disposable {

class RefCountDisposable : public DisposableBase, public std::enable_shared_from_this<RefCountDisposable> {

public:
    disposable_t underlying_disposable;
    bool is_primary_disposed = false;
    bool is_disposed = false;
    RLock lock;
    int count = 0;

protected:
    RefCountDisposable(const disposable_t& disposable) 
        : underlying_disposable(disposable) {}
public:
    ~RefCountDisposable(){ this->dispose(); }
    inline static std::shared_ptr<RefCountDisposable> get(const disposable_t& disposable_) {
        return std::shared_ptr<RefCountDisposable>(new RefCountDisposable(disposable_));
    }
    inline std::shared_ptr<RefCountDisposable> getptr() { return shared_from_this(); }

    void dispose() override;

    void release();
    disposable_t get_disposable();

    class InnerDisposable : public DisposableBase {

    public:
        std::shared_ptr<RefCountDisposable> parent;
        bool is_disposed = false;
        RLock lock;

    public:
        InnerDisposable(const std::shared_ptr<RefCountDisposable>& parent_) 
            : parent(parent_) {}
        ~InnerDisposable(){ this->dispose(); }

        void dispose() override;

    }; // END class RefCountDisposable::InnerDisposable

}; // END class RefCountDisposable

} // END namespace rx::disposable

#endif // RX_DISPOSABLE_REFCOUNTDISPOSABLE_H