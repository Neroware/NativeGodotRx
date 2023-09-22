#ifndef RX_TEMPLATES_DISPOSABLE_H
#define RX_TEMPLATES_DISPOSABLE_H

#include "wrapper/templates.h"

#include "abstract/disposable.h"

using namespace rx::wrappers;
using namespace rx::abstract;

namespace rx::templates {

class Disposable_ : public DisposableBase, public std::enable_shared_from_this<Disposable_> {

private:
    Ref<RxDisposableTemplate_> self;
protected:
    Disposable_(){}
public:
    ~Disposable_(){}
    inline disposable_ getptr() { return this->shared_from_this(); }
    inline static disposable_ get() { return std::shared_ptr<Disposable_>(new Disposable_()); }
    inline void _template(Ref<RxDisposableTemplate_> t) { this->self = t; }

    inline void dispose() override {
        if (_has_template_method(self, "_dispose")) {
            RX_TEMPLATE_CALL(_dispose, self, getptr());
            return;
        }
        basic::noop<>();
    }

    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Disposable>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

} // END namespace rx::templates

#endif // RX_TEMPLATES_DISPOSABLE_H