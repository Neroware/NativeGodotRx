#include "observable/definitions.h"

#include "observable/connectableobservable.h"
#include "disposable/compositedisposable.h"

#include "subject/subject.h"

using namespace rx::subject;
using namespace rx::disposable;

namespace rx::observable::op {

observable_op_t ops::multicast_(
    const rx_subject_t& subject
) {
    observable_op_t multicast = OP(source) {
        if (!subject) {
            throw BadArgumentException("multicast: Subject cannot be NULL");
        }
        auto ret = ConnectableObservable::get(source, subject);
        return ret;
    };

    return multicast;
}

observable_op_t ops::multicast_(
    const rx_subject_factory_t& subject_factory,
    const mapper_t<rx_observable_t, rx_observable_t>& mapper
) {
    observable_op_t multicast = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            assert(subject_factory);
            auto connectable = std::static_pointer_cast<ConnectableObservable>(
                source->pipe(
                    multicast_(subject_factory(scheduler))
                )
            );
            assert(mapper);
            auto subscription = mapper(connectable)->subscribe(
                observer, scheduler
            );

            return std::make_shared<CompositeDisposable>(subscription, connectable->connect(scheduler));
        };

        return Observable::get(subscribe);
    };

    return multicast;
}

} // END namespace rx::observable::op