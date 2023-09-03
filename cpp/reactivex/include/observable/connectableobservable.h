#ifndef RX_OBSERVABLE_CONNECTABLEOBSERVABLE_H
#define RX_OBSERVABLE_CONNECTABLEOBSERVABLE_H

#include "observable/observable.h"

#include "typing.h"
#include "internal/utils.h"

namespace rx::observable {

class ConnectableObservable : public Observable {

public:
    subject_t subject;
    bool has_subscription = false;
    disposable_t subscription;
    observable_t source;

protected:
    ConnectableObservable(const observable_t& source_, const subject_t& subject_) 
        : source(source_), subscription(nullptr), subject(subject_) {}
public:
    ~ConnectableObservable(){}
    inline static std::shared_ptr<ConnectableObservable> get(const observable_t& source_, const subject_t& subject_) { return std::shared_ptr<ConnectableObservable>(new ConnectableObservable(source_, subject_)); }
    inline std::shared_ptr<ConnectableObservable> getptr() { return std::static_pointer_cast<ConnectableObservable>(Observable::getptr()); }

private:
    virtual disposable_t _subscribe_core(const observer_t& observer, const scheduler_t& scheduler = nullptr) override;
public:
    disposable_t connect(const scheduler_t& scheduler = nullptr);
    rx_observable_t auto_connect(uint64_t subscriber_count = 1);


}; // END class ConnectableObservable

} // rx::observable

#endif // RX_OBSERVABLE_CONNECTABLEOBSERVABLE_H