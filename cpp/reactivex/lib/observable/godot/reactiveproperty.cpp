#include "observable/reactiveproperty.h"

#include "disposable/disposable.h"

namespace rx::observable {

ReactiveProperty::ReactiveProperty(const Variant& initial_value_, bool distinct_until_changed_, bool raise_latest_value_on_subscribe_) :
    _latest_value(initial_value_),
    _distinct_until_changed(distinct_until_changed_),
    _raise_latest_value_on_subscribe(raise_latest_value_on_subscribe_), 
    Observable(SUBSCRIBE(observer, scheduler = nullptr) {
        if (this->is_disposed) {
            observer->on_completed();
            return std::make_shared<Disposable>();
        }

        {
            std::unique_lock<std::shared_mutex> lock(this->_mutex);
            this->_observers.push_back(observer);
        }

        if (this->_raise_latest_value_on_subscribe) {
            observer->on_next(this->_latest_value);
        }

        auto self = getptr();

        dispose_t dispose = [self, observer](){
            std::unique_lock<std::shared_mutex> lock(self->_mutex);
            self->_observers.remove(observer);
        };

        return std::make_shared<Disposable>(dispose);
    }) {}

Variant ReactiveProperty::_get_value() const {
    return this->_latest_value;
}

void ReactiveProperty::_set_value(const Variant& value) {
    if (this->_distinct_until_changed && this->_latest_value == value)
        return;
    this->_latest_value = value;

    std::list<observer_t> observers_;
    {
        std::shared_lock<std::shared_mutex> lock(this->_mutex);
        observers_ = this->_observers;
    }
    for (const auto& obs : observers_) {
        obs->on_next(this->_latest_value);
    }
}

String ReactiveProperty::to_string() const {
    if (this->is_disposed)
        return "<<Disposed ReactiveProperty>>";
    return UtilityFunctions::str(this->_latest_value);
}

bool ReactiveProperty::equals(const Variant& other) const {
    return this->_latest_value == other;
}

void ReactiveProperty::dispose() {
    if (this->is_disposed)
        return;
    
    this->is_disposed = true;

    std::list<observer_t> observers_;
    {
        std::shared_lock<std::shared_mutex> lock(this->_mutex);
        observers_ = this->_observers;
    }
    for (const auto& obs : observers_) {
        obs->on_completed();
    }

    {
        std::unique_lock<std::shared_mutex> lock(this->_mutex);
        this->_observers.clear();
    }

    if (this->_source_subscription) {
        this->_source_subscription->dispose();
    }
}

rx_readonly_reactive_property_t ReactiveProperty::to_readonly() {
    throw NotImplementedException();
}

rx_reactive_property_t ReactiveProperty::get(const Variant& initial_value_, bool distinct_until_changed_, bool raise_latest_value_on_subscribe_, const rx_observable_t& source) { 
    auto self = rx_reactive_property_t(new ReactiveProperty(
        initial_value_, 
        distinct_until_changed_, 
        raise_latest_value_on_subscribe_
    ));
    if (source) {
        self->_source_subscription = source->subscribe([self](const Variant& value) { self->_set_value(value); });
    }
    return self;
}
    
rx_reactive_property_t ReactiveProperty::from_get_set(
    const std::function<Variant()>& getter,
    const std::function<void(const Variant&)>& setter,
    bool distinct_until_changed_,
    bool raise_latest_value_on_subscribe_
) {
    auto prop = ReactiveProperty::get(
        getter(),
        distinct_until_changed_,
        raise_latest_value_on_subscribe_
    );

    prop->_source_subscription = prop->subscribe(
        [setter](const Variant& x) { setter(x); }
    );
    return prop;
}

rx_reactive_property_t ReactiveProperty::from_member(
    const Variant& target,
    const StringName& member_name,
    const mapper_t<Variant, Variant>& convert,
    const mapper_t<Variant, Variant>& convert_back,
    bool distinct_until_changed_, 
    bool raise_latest_value_on_subscribe_
) {
    auto getter = [target, member_name]() {
        bool valid = true;
        return target.get_named(member_name, valid);
    };
    auto setter = [target, member_name](const Variant& value) {
        Variant _target = target;
        bool valid = true;
        _target.set_named(member_name, value, valid);
    };

    auto _convert = convert ? convert : basic::identity<const Variant&>;
    auto _convert_back = convert_back ? convert_back : basic::identity<const Variant&>;

    return from_get_set(
        [_convert, getter]() { return _convert(getter()); },
        [_convert_back, setter](const Variant& x) { setter(_convert_back(x)); },
        distinct_until_changed_,
        raise_latest_value_on_subscribe_
    );
}

rx_readonly_reactive_property_t ReactiveProperty::derived(
    const rx_readonly_reactive_property_t& p, 
    const mapper_t<Variant, Variant>& fn
) {
    throw NotImplementedException();
}

rx_readonly_reactive_property_t ReactiveProperty::computed(
    const mapper_t<Variant, Variant>& fn,
    const rx_readonly_reactive_property_t& p...
) {
    throw NotImplementedException();
}
rx_readonly_reactive_property_t ReactiveProperty::computed(
    const mapper_t<Variant, Variant>& fn,
    const RxList<rx_readonly_reactive_property_t>& p_list
) {
    throw NotImplementedException();
}

} // END namespace rx::observable