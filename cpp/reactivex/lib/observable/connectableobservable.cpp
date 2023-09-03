#include "observable/connectableobservable.h"

#include "disposable/compositedisposable.h"
#include "disposable/disposable.h"

using namespace rx::disposable;

namespace rx::observable {

disposable_t ConnectableObservable::_subscribe_core(const observer_t& observer, const scheduler_t& scheduler) {
    return this->subject->subscribe(observer, scheduler);
}

disposable_t ConnectableObservable::connect(const scheduler_t& scheduler) {
    auto self = this->getptr();

    if (!this->has_subscription) {
        this->has_subscription = true;

        dispose_t dispose = [self]() {
            self->has_subscription = false;
        };

        auto _subscription = this->source->subscribe(this->subject, scheduler);
        this->subscription = std::make_shared<CompositeDisposable>(_subscription, std::make_shared<Disposable>(dispose));
    }

    return this->subscription;
}

rx_observable_t ConnectableObservable::auto_connect(uint64_t subscriber_count) {
    auto connectable_subscription = std::make_shared<disposable_t>();
    auto count = std::make_shared<uint64_t>(0);
    auto source = getptr();
    auto is_connected = std::make_shared<bool>(false);

    if (subscriber_count == 0) {
        *connectable_subscription = source->connect();
        *is_connected = true;
    }

    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        ++(*count);
        bool should_connect = *count == subscriber_count && !*is_connected;
        auto subscription = source->subscribe(observer);
        if (should_connect) {
            *connectable_subscription = source->connect(scheduler);
            *is_connected = true;
        }

        dispose_t dispose = [=]() {
            subscription->dispose();
            --(*count);
            *is_connected = false;
        };

        return std::make_shared<Disposable>(dispose);
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable