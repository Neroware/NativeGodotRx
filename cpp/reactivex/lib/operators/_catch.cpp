#include "observable/definitions.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/serialdisposable.h"

namespace rx::observable::op {

using handler_t = std::function<rx_observable_t(const error_t&, const rx_observable_t&)>;

static rx_observable_t catch_handler(
    const rx_observable_t& source,
    const handler_t& handler
) {
    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        auto d1 = std::make_shared<SingleAssignmentDisposable>();
        auto subscription = std::make_shared<SerialDisposable>();

        subscription->set_disposable(d1);

        on_error_t on_error = [=](const std::exception_ptr& exception) {
            std::shared_ptr<Observable> result;
            try {
                result = handler(exception, source);
                if (result == nullptr) {
                    throw std::runtime_error("Handler returned nullptr");
                }
            }
            catch (...) {
                observer->on_error(std::current_exception());
                return;
            }
                
            auto d = std::make_shared<SingleAssignmentDisposable>();
            subscription->set_disposable(d);
            d->set_disposable(result->subscribe(observer, scheduler));
        };

        d1->set_disposable(source->subscribe(
            ONNEXT_FWD(observer), on_error, ONCOMPLETED_FWD(observer), 
            scheduler
        ));
        return subscription;
    };

    return Observable::get(subscribe);
};

observable_op_t ops::catch_(const rx_observable_t& handler) {
    auto _catch = OP(source) {
        return catch_with_iterable(observable_list_t{source, handler});
    };
    return _catch;
}
observable_op_t ops::catch_(const handler_t& handler) {
    auto _catch = OP(source) {
        return catch_handler(source, handler);
    };
    return _catch;
}

} // END namespace rx::observable::op