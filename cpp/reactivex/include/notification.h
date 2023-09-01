#ifndef RX_NOTIFICATION_H
#define RX_NOTIFICATION_H

#include "abstract/observer.h"
#include "abstract/observable.h"
#include "exception/exceptionwrapper.h"

using namespace godot;
using namespace rx::abstract;

namespace rx {

class notification_t : public std::enable_shared_from_this<notification_t> {
public:
    bool has_value = false;
    Variant value;
    String kind = "";

protected:
    notification_t(){}
public:
    inline std::shared_ptr<notification_t> getptr() { return shared_from_this(); }
    ~notification_t(){}

    virtual void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const = 0;
    virtual void accept(const std::shared_ptr<ObserverBase>& observer) const = 0;
    virtual String to_string() const { throw NotImplementedException(); }

    std::shared_ptr<ObservableBase> to_observable(const std::shared_ptr<SchedulerBase>& scheduler = nullptr);

    /* Dumb comparison, which is just laughably slow... Too bad! */
    inline bool operator==(const notification_t& other) const { return this->kind == other.kind; }

}; // END class notification_t 

class notification_on_next_t : public notification_t {
protected:
    notification_on_next_t(const Variant& value = Variant()) {
        this->has_value = true;
        this->value = value;
        this->kind = "N";
    }
public:
    inline static std::shared_ptr<notification_on_next_t> get(const Variant& value = Variant()) { return std::shared_ptr<notification_on_next_t>(new notification_on_next_t(value)); }
    inline std::shared_ptr<notification_on_next_t> getptr() { return std::static_pointer_cast<notification_on_next_t>(notification_t::getptr()); }
    ~notification_on_next_t() {}

    void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const override;
    void accept(const std::shared_ptr<ObserverBase>& observer) const override;
    String to_string() const override;
    
}; // END class notification_on_next_t 

class notification_on_error_t : public notification_t {
public:
    std::exception_ptr exception;

protected:
    notification_on_error_t(const std::exception_ptr& e) : exception(e) { this->kind = "E"; }
    notification_on_error_t(const std::string& error) : exception(std::make_exception_ptr(rx_exception(error))) { this->kind = "E"; }
    notification_on_error_t() : exception(std::make_exception_ptr(rx_exception())) { this->kind = "E"; }
public:
    inline static std::shared_ptr<notification_on_error_t> get(const std::exception_ptr& e) { return std::shared_ptr<notification_on_error_t>(new notification_on_error_t(e)); }
    inline static std::shared_ptr<notification_on_error_t> get(const std::string& e) { return std::shared_ptr<notification_on_error_t>(new notification_on_error_t(e)); }
    inline static std::shared_ptr<notification_on_error_t> get() { return std::shared_ptr<notification_on_error_t>(new notification_on_error_t()); }
    inline std::shared_ptr<notification_on_error_t> getptr() { return std::static_pointer_cast<notification_on_error_t>(notification_t::getptr()); }
    ~notification_on_error_t() {}

    void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const override;
    void accept(const std::shared_ptr<ObserverBase>& observer) const override;
    String to_string() const override;

}; // END class notification_on_error_t 

class notification_on_completed_t : public notification_t {
protected:
    notification_on_completed_t() { this->kind = "C"; }
public:
    inline static std::shared_ptr<notification_on_completed_t> get() { return std::shared_ptr<notification_on_completed_t>(new notification_on_completed_t()); }
    inline std::shared_ptr<notification_on_completed_t> getptr() { return std::static_pointer_cast<notification_on_completed_t>(notification_t::getptr()); }
    ~notification_on_completed_t() {}

    void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const override;
    void accept(const std::shared_ptr<ObserverBase>& observer) const override;
    String to_string() const override;

}; // END class notification_on_completed_t 

typedef std::function<void(const std::shared_ptr<notification_t>&)> notification_handler_t;

notification_handler_t notification_handler_cb(const Callable& handler);
std::shared_ptr<ObserverBase> from_notifier(const notification_handler_t& handler);

} // END namespace rx

#endif // RX_NOTIFICATION_H