#ifndef RX_TEMPLATES_LOCK_H
#define RX_TEMPLATES_LOCK_H

#include "wrapper/templates.h"

#include "abstract/lock.h"

using namespace rx::wrappers;
using namespace rx::abstract;

namespace rx::templates {

class Lock_ : public LockBase, public std::enable_shared_from_this<Lock_> {

private:
    Ref<RxLockTemplate_> self;
protected:
    Lock_(){}
public:
    ~Lock_(){}
    inline lock_ getptr() { return this->shared_from_this(); }
    inline static lock_ get() { return std::shared_ptr<Lock_>(new Lock_()); }
    inline void _template(Ref<RxLockTemplate_> t) { this->self = t; }

    inline void lock() override {
        RX_TEMPLATE_CALL(_lock, self, getptr());
    }
    inline void unlock() override {
        RX_TEMPLATE_CALL(_unlock, self, getptr());
    }
    inline bool try_lock() override {
        return RX_TEMPLATE_CALL(_try_lock, self, getptr());
    }

    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Lock>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

} // END namespace rx::templates

#endif // RX_TEMPLATES_LOCK_H