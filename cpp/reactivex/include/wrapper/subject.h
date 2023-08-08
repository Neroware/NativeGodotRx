#ifndef RX_WRAPPER_SUBJECT_H
#define RX_WRAPPER_SUBJECT_H

#include "wrapper/wrapper.h"

#include "wrapper/observer.h"
#include "wrapper/scheduler.h"

#include "abstract/observable.h"
#include "abstract/observer.h"
#include "abstract/subject.h"

#include "observable/observable.h"
#include "exception/exception.h"
#include "exception/exceptionwrapper.h"

#include "observable/definitions.h"

#include "cast.h"
#include "basic.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::observable;

namespace rx {
namespace wrappers {

class RxSubject : public RefCounted {
    GDCLASS(RxSubject, RefCounted)
    RX_ABSTRACT_WRAPPER(RxSubject, SubjectBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxSubject::equals);
        {
		    MethodInfo mi;
		    mi.name = "subscribe";
		    ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "subscribe", &RxSubject::subscribe, mi);
	    }
        ClassDB::bind_method(D_METHOD("on_next", "item"), &RxSubject::on_next);
        ClassDB::bind_method(D_METHOD("on_error", "error"), &RxSubject::on_error);
        ClassDB::bind_method(D_METHOD("on_completed"), &RxSubject::on_completed);
    }
public:
    Ref<RxDisposable> subscribe(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
    void on_next(const Variant& item);
    void on_error(Ref<RxError> error);
    void on_completed();

};

} // END namespace wrapper

using namespace rx::wrappers;

} // END namespace rx

#endif // RX_WRAPPER_SUBJECT_H