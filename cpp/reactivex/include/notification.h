#ifndef RX_NOTIFICATION_H
#define RX_NOTIFICATION_H

#include "abstract/observer.h"
#include "abstract/observable.h"
#include "exception/exceptionwrapper.h"

using namespace godot;
using namespace rx::abstract;

namespace rx {

class Notification : public std::enable_shared_from_this<Notification> {
public:
    bool has_value = false;
    Variant value;
    String kind = "";

protected:
    Notification(){}
public:
    inline std::shared_ptr<Notification> getptr() { return shared_from_this(); }
    ~Notification(){}

    virtual void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const = 0;
    virtual void accept(const observer_t& observer) const = 0;
    virtual String to_string() const { throw NotImplementedException(); }

    observable_t to_observable(const scheduler_t& scheduler = nullptr);

    /* Dumb comparison, which is just laughably slow... Too bad! */
    inline bool operator==(const Notification& other) const { return this->kind == other.kind; }

}; // END class Notification 

class NotificationOnNext : public Notification {
protected:
    NotificationOnNext(const Variant& value = Variant()) {
        this->has_value = true;
        this->value = value;
        this->kind = "N";
    }
public:
    inline static std::shared_ptr<NotificationOnNext> get(const Variant& value = Variant()) { return std::shared_ptr<NotificationOnNext>(new NotificationOnNext(value)); }
    inline std::shared_ptr<NotificationOnNext> getptr() { return std::static_pointer_cast<NotificationOnNext>(Notification::getptr()); }
    ~NotificationOnNext() {}

    void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const override;
    void accept(const observer_t& observer) const override;
    String to_string() const override;
    
}; // END class NotificationOnNext 

class NotificationOnError : public Notification {
public:
    std::exception_ptr exception;

protected:
    NotificationOnError(const std::exception_ptr& e) : exception(e) { this->kind = "E"; }
    NotificationOnError(const std::string& error) : exception(std::make_exception_ptr(rx_exception(error))) { this->kind = "E"; }
    NotificationOnError() : exception(std::make_exception_ptr(rx_exception())) { this->kind = "E"; }
public:
    inline static std::shared_ptr<NotificationOnError> get(const std::exception_ptr& e) { return std::shared_ptr<NotificationOnError>(new NotificationOnError(e)); }
    inline static std::shared_ptr<NotificationOnError> get(const std::string& e) { return std::shared_ptr<NotificationOnError>(new NotificationOnError(e)); }
    inline static std::shared_ptr<NotificationOnError> get() { return std::shared_ptr<NotificationOnError>(new NotificationOnError()); }
    inline std::shared_ptr<NotificationOnError> getptr() { return std::static_pointer_cast<NotificationOnError>(Notification::getptr()); }
    ~NotificationOnError() {}

    void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const override;
    void accept(const observer_t& observer) const override;
    String to_string() const override;

}; // END class NotificationOnError 

class NotificationOnCompleted : public Notification {
protected:
    NotificationOnCompleted() { this->kind = "C"; }
public:
    inline static std::shared_ptr<NotificationOnCompleted> get() { return std::shared_ptr<NotificationOnCompleted>(new NotificationOnCompleted()); }
    inline std::shared_ptr<NotificationOnCompleted> getptr() { return std::static_pointer_cast<NotificationOnCompleted>(Notification::getptr()); }
    ~NotificationOnCompleted() {}

    void accept(const on_next_t& on_next, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) const override;
    void accept(const observer_t& observer) const override;
    String to_string() const override;

}; // END class NotificationOnCompleted 

observer_t from_notifier(const notification_handler_t& handler);

} // END namespace rx

#endif // RX_NOTIFICATION_H