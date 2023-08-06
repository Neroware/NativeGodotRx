#ifndef RX_NOTIFICATION_H
#define RX_NOTIFICATION_H

#include "abstract/observer.h"
#include "abstract/observable.h"
#include "exception/exceptionwrapper.h"

using namespace godot;
using namespace rx::abstract;

namespace rx {

struct notification_t {
    bool has_value = false;
    Variant value;
    String kind = "";

    notification_t(){}
    ~notification_t(){}

    virtual void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const = 0;
    virtual void accept(const std::shared_ptr<ObserverBase>& observer) const = 0;
    virtual String to_string() const { throw NotImplementedException(); }

    std::shared_ptr<ObservableBase> to_observable(const std::shared_ptr<SchedulerBase>& scheduler = nullptr);

    /* Dumb comparison, which is just laughably slow... Too bad! */
    inline bool operator==(const notification_t& other) const { return this->kind == other.kind; }

}; // END struct notification_t 

struct notification_on_next_t : public notification_t {
    notification_on_next_t(const Variant& value = Variant()) {
        this->has_value = true;
        this->value = value;
        this->kind = "N";
    }
    ~notification_on_next_t() {}

    void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const override;
    void accept(const std::shared_ptr<ObserverBase>& observer) const override;
    String to_string() const override;
    
}; // END struct notification_on_next_t 

struct notification_on_error_t : public notification_t {
    std::exception exception;

    notification_on_error_t(const std::exception& e) : exception(e) { this->kind = "E"; }
    notification_on_error_t(const std::string& error) : exception(Exception(error)) { this->kind = "E"; }
    notification_on_error_t() : exception(Exception()) { this->kind = "E"; }
    ~notification_on_error_t() {}

    void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const override;
    void accept(const std::shared_ptr<ObserverBase>& observer) const override;
    String to_string() const override;

}; // END struct notification_on_error_t 

struct notification_on_completed_t : public notification_t {
    notification_on_completed_t() { this->kind = "C"; }
    ~notification_on_completed_t() {}

    void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const override;
    void accept(const std::shared_ptr<ObserverBase>& observer) const override;
    String to_string() const override;

}; // END struct notification_on_completed_t 

typedef std::function<void(const notification_t&)> notification_handler_t;
static std::shared_ptr<ObserverBase> from_notifier(const notification_handler_t& handler);

} // END namespace rx

#endif // RX_NOTIFICATION_H