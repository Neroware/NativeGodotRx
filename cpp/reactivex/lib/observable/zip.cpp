#include "observable/definitions.h"

#include "disposable/compositedisposable.h"
#include "disposable/singleassignmentdisposable.h"

using namespace rx::disposable;

namespace rx::observable {

rx_observable_t obs::zip_(const rx_observable_list_t& sources_) {
    observable_vec_t sources(sources_.begin(), sources_.end());

    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
       size_t n = sources.size();
       std::shared_ptr<Array[]> queues(new Array[n]());
       auto lock = std::make_shared<RLock>();
       std::shared_ptr<bool[]> is_completed(new bool[n]{ 0 });

        auto next_ = [=](int i) {
            std::lock_guard<RLock> guard(*lock);
            if (all(queues, n, std::function<bool(const Array&)>([](const Array& elem){ return !elem.is_empty(); } ))) {
                std::vector<Variant> queue_values(n);
                try {
                    for (int j = 0; j < n; j++) {
                        queue_values[j] = queues[j].pop_front();
                    }
                }
                catch(...) {
                    observer->on_error(std::current_exception());
                    return;
                }

                Array res = tuple(queue_values, n);
                observer->on_next(res);

                // after sending the zipped values, complete the 
                // observer if at least one
                // upstream observable is completed and its queue has 
                // length zero
                for (int j = 0; j < n; j++) {
                    Array queue = queues[j];
                    bool done = is_completed[j];
                    if (done && queue.size() == 0) {
                        observer->on_completed();
                        break;
                    }
                }
            }
        };

        auto completed = [=](int i) {
            is_completed[i] = true;
            if (queues[i].size() == 0) {
                observer->on_completed();
            }
        };

        auto subscriptions = std::make_shared<disposable_vec_t>(n);

        auto func = [=](int i) {
            rx_observable_t source = sources[i];
            
            auto sad = std::make_shared<SingleAssignmentDisposable>();

            on_next_t on_next = [=](const Variant& x) {
                queues[i].append(x);
                next_(i);
            };

            sad->set_disposable(source->subscribe(
                on_next, ONERROR_FWD(observer), [=]() { completed(i); },
                scheduler
            ));
            (*subscriptions)[i] = sad;
        };

        for (int idx = 0; idx < n; idx++) {
            func(idx);
        }

        return std::make_shared<CompositeDisposable>(*subscriptions);
    };

    return Observable::get(subscribe);
}

rx_observable_t obs::zip_(const rx_observable_t& sources...) {
    return zip_({sources});
}

}