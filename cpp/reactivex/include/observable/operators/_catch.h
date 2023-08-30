#ifndef RX_OBSERVABLE_OPERATORS_CATCH_H
#define RX_OBSERVABLE_OPERATORS_CATCH_H

#include "observable/observable.h"
#include "observable/catch.h"
#include "disposable/singleassignmentdisposable.h"
#include "disposable/serialdisposable.h"

#include "typing.h"
#include "internal/utils.h"

namespace rx::observable {

using handler_t = std::function<std::shared_ptr<Observable>(const std::exception_ptr&, const std::shared_ptr<Observable>&)>;

static std::shared_ptr<Observable> catch_handler(
    const std::shared_ptr<Observable>& source,
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

static observable_op_t catch_(const std::shared_ptr<Observable>& handler) {
    auto _catch = OP(source) {
        return catch_with_iterable_(observable_list_t{source, handler});
    };
    return _catch;
}
static observable_op_t catch_(const handler_t& handler) {
    auto _catch = OP(source) {
        return catch_handler(source, handler);
    };
    return _catch;
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_OPERATORS_CATCH_H
