#include "observable/definitions.h"

#include "disposable/compositedisposable.h"

namespace rx::observable {

observable_op_t godot::process_time_interval_(double initial_time) {
    observable_op_t process_time_interval = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : CurrentThreadScheduler::singleton();

            auto dt = std::make_shared<double>(initial_time);

            on_next_t on_process = [dt](double delta) {
                *dt += delta;
            };
            auto process_sub = on_idle_frame()->subscribe(on_process);

            on_next_t on_next = [dt, observer](const Variant& value) {
                double span = *dt;
                *dt = 0.0;
                observer->on_next(Array::make(value, span));
            };

            auto sub = source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), _scheduler
            );

            return std::make_shared<CompositeDisposable>(process_sub, sub);
        };

        return Observable::get(subscribe);
    };

    return process_time_interval;
}

observable_op_t godot::physics_time_interval_(double initial_time) {
    observable_op_t physics_time_interval = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : CurrentThreadScheduler::singleton();

            auto dt = std::make_shared<double>(initial_time);

            on_next_t on_process = [dt](double delta) {
                *dt += delta;
            };
            auto process_sub = on_physics_step()->subscribe(on_process);

            on_next_t on_next = [dt, observer](const Variant& value) {
                double span = *dt;
                *dt = 0.0;
                observer->on_next(Array::make(value, span));
            };

            auto sub = source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), _scheduler
            );

            return std::make_shared<CompositeDisposable>(process_sub, sub);
        };

        return Observable::get(subscribe);
    };

    return physics_time_interval;
}

} // END name