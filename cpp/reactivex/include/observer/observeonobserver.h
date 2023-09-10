#ifndef RX_OBSERVER_OBSERVERONOBSERVER_H
#define RX_OBSERVER_OBSERVERONOBSERVER_H

#include "observer/scheduledobserver.h"

namespace rx::observer {

class ObserveOnObserver : public ScheduledObserver {

protected:
    ObserveOnObserver(const scheduler_t& scheduler, const observer_t& observer) 
        : ScheduledObserver(scheduler, observer) {}
public:
    inline std::shared_ptr<ObserveOnObserver> getptr() { return std::static_pointer_cast<ObserveOnObserver>(ScheduledObserver::getptr()); }
    inline static std::shared_ptr<ObserveOnObserver> get(const scheduler_t& scheduler, const observer_t& observer) {
        return std::shared_ptr<ObserveOnObserver>(new ObserveOnObserver(scheduler, observer)); 
    }

    void _on_next_core(const Variant& item) override;
    void _on_error_core(const error_t& error) override;
    void _on_completed_core() override;

};

} // END rx::observer

#endif // RX_OBSERVER_OBSERVERONOBSERVER_H