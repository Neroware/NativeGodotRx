#include "observable/readonlyreactiveproperty.h"

#include "disposable/disposable.h"

namespace rx::observable {

ReadOnlyReactiveProperty::ReadOnlyReactiveProperty(
    const Variant& initial_value_, 
    bool distinct_until_changed_, 
    bool raise_latest_value_on_subscribe_
) :
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

Variant ReadOnlyReactiveProperty::_get_value() const {
    return this->_latest_value;
}

String ReadOnlyReactiveProperty::to_string() const {
    if (this->is_disposed)
        return "<<Disposed ReadOnlyReactiveProperty>>";
    return UtilityFunctions::str(this->_latest_value);
}

bool ReadOnlyReactiveProperty::equals(const Variant& other) const {
    return this->_latest_value == other;
}

void ReadOnlyReactiveProperty::dispose() {
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

void ReadOnlyReactiveProperty::on_next(const Variant& value) {
    if (this->_distinct_until_changed && this->_latest_value == value)
		return;
    this->_latest_value = value;

    std::list<observer_t> observers_;
    {
        std::shared_lock<std::shared_mutex> lock(this->_mutex);
        observers_ = this->_observers;
    }
    for (const auto& obs : observers_) {
        obs->on_next(value);
    }
}
void ReadOnlyReactiveProperty::on_error(const error_t& e) {
    std::list<observer_t> observers_;
    {
        std::shared_lock<std::shared_mutex> lock(this->_mutex);
        observers_ = this->_observers;
    }
    for (const auto& obs : observers_) {
        obs->on_error(e);
    }
}
void ReadOnlyReactiveProperty::on_completed() {
    this->dispose();
}

rx_readonly_reactive_property_t ReadOnlyReactiveProperty::get(const rx_observable_t& source, const Variant& initial_value_, bool distinct_until_changed_, bool raise_latest_value_on_subscribe_) { 
    auto self = rx_readonly_reactive_property_t(new ReadOnlyReactiveProperty(
        initial_value_, 
        distinct_until_changed_, 
        raise_latest_value_on_subscribe_
    ));
    if (source) {
        self->_source_subscription = source->subscribe(self);
    }
    return self;
}

} // END namespace rx::observable