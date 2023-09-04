#ifndef RX_OBSERVABLE_OPERATORS_MULTICAST_H
#define RX_OBSERVABLE_OPERATORS_MULTICAST_H

#include "observable/observable.h"

#include "observable/connectableobservable.h"
#include "disposable/compositedisposable.h"

#include "subject/subject.h"
#include "internal/utils.h"

using namespace rx::subject;
using namespace rx::disposable;

namespace rx::observable::op {

typedef std::function<rx_subject_t(const scheduler_t&)> rx_subject_factory_t;

static observable_op_t multicast_(
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

static observable_op_t multicast_(
    const rx_subject_factory_t& subject_factory,
    const mapper_t<rx_observable_t, rx_observable_t>& mapper = nullptr
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

#endif // RX_OBSERVABLE_OPERATORS_MULTICAST_H