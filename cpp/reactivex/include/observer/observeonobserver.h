#ifndef RX_OBSERVER_OBSERVERONOBSERVER_H
#define RX_OBSERVER_OBSERVERONOBSERVER_H

#include "observer/scheduledobserver.h"

namespace rx::observer {

class ObserveOnObserver : public ScheduledObserver {

protected:
    ObserveOnObserver(const std::shared_ptr<SchedulerBase>& scheduler, const std::shared_ptr<ObserverBase>& observer) 
        : ScheduledObserver(scheduler, observer) {}
public:
    inline std::shared_ptr<ObserveOnObserver> getptr() { return std::static_pointer_cast<ObserveOnObserver>(ScheduledObserver::getptr()); }
    inline static std::shared_ptr<ObserveOnObserver> get(const std::shared_ptr<SchedulerBase>& scheduler, const std::shared_ptr<ObserverBase>& observer) {
        return std::shared_ptr<ObserveOnObserver>(new ObserveOnObserver(scheduler, observer)); 
    }

    void _on_next_core(const Variant& item) override;
    void _on_error_core(const std::exception& error) override;
    void _on_completed_core() override;

};

} // END rx::observer

#endif // RX_OBSERVER_OBSERVERONOBSERVER_H