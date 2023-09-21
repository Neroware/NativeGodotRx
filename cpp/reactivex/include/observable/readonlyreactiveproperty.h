#ifndef RX_OBSERVABLE_READONLYREACTIVEPROPERTY_H
#define RX_OBSERVABLE_READONLYREACTIVEPROPERTY_H

#include "observable/observable.h"

namespace rx::observable {

class ReadOnlyReactiveProperty : public Observable, public DisposableBase, public ObserverBase {

private:
    std::shared_mutex _mutex;

    Variant _latest_value;
    disposable_t _source_subscription;
    std::list<observer_t> _observers;
    bool _distinct_until_changed;
    bool _raise_latest_value_on_subscribe;

public:
    bool is_disposed = false;

protected:
    ReadOnlyReactiveProperty(const Variant& initial_value_, bool distinct_until_changed_ = true, bool raise_latest_value_on_subscribe_ = true);
public:
    ~ReadOnlyReactiveProperty() { this->dispose(); }
    static rx_readonly_reactive_property_t get(const rx_observable_t& source, const Variant& initial_value_, bool distinct_until_changed_ = true, bool raise_latest_value_on_subscribe_ = true);
    inline rx_readonly_reactive_property_t getptr() { return std::static_pointer_cast<ReadOnlyReactiveProperty>(Observable::getptr()); }

    virtual Variant _get_value() const;
    String to_string() const;
    bool equals(const Variant& other) const;

    virtual void dispose() override;

    virtual void on_next(const Variant& i) override;
    virtual void on_error(const error_t& e) override;
    virtual void on_completed() override;

}; // END class ReactiveProperty

}; // END namespace rx::observable

#endif // RX_OBSERVABLE_READONLYREACTIVEPROPERTY_H