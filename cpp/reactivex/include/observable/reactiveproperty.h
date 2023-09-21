#ifndef RX_OBSERVABLE_REACTIVEPROPERTY_H
#define RX_OBSERVABLE_REACTIVEPROPERTY_H

#include "observable/observable.h"

namespace rx::observable {

class ReactiveProperty : public Observable, public DisposableBase {

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
    ReactiveProperty(const Variant& initial_value_, bool distinct_until_changed_ = true, bool raise_latest_value_on_subscribe_ = true);

public:
    ~ReactiveProperty(){ this->dispose(); }
    static rx_reactive_property_t get(const Variant& initial_value_, bool distinct_until_changed_ = true, bool raise_latest_value_on_subscribe_ = true, const rx_observable_t& source = nullptr);
    inline rx_reactive_property_t getptr() { return std::static_pointer_cast<ReactiveProperty>(Observable::getptr()); }

    virtual Variant _get_value() const;
    virtual void _set_value(const Variant& value);
    String to_string() const;
    bool equals(const Variant& other) const;
    virtual void dispose() override;
    rx_readonly_reactive_property_t to_readonly();
    
    static rx_reactive_property_t from_get_set(
        const std::function<Variant()>& getter,
        const std::function<void(const Variant&)>& setter,
        bool distinct_until_changed_ = true,
        bool raise_latest_value_on_subscribe_ = true
    );

    static rx_reactive_property_t from_member(
        const Variant& target,
        const StringName& member_name,
        const mapper_t<Variant, Variant>& convert = nullptr,
        const mapper_t<Variant, Variant>& convert_back = nullptr,
        bool distinct_until_changed_ = true, 
        bool raise_latest_value_on_subscribe_ = true
    );

    static rx_readonly_reactive_property_t derived(
        const rx_readonly_reactive_property_t& p, 
        const mapper_t<Variant, Variant>& fn
    );

    static rx_readonly_reactive_property_t computed(
        const mapper_t<Variant, Array>& fn,
        const rx_readonly_reactive_property_t& p...
    );
    static rx_readonly_reactive_property_t computed(
        const mapper_t<Variant, Array>& fn,
        const RxList<rx_readonly_reactive_property_t>& p_list
    );

}; // END class ReactiveProperty

}; // END namespace rx::observable

#endif // RX_OBSERVABLE_REACTIVEPROPERTY_H