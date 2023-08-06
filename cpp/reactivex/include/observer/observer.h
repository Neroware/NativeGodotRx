#ifndef RX_OBSERVER_OBSERVER_H
#define RX_OBSERVER_OBSERVER_H

#include "abstract/observer.h"
#include "abstract/disposable.h"

#include "disposable/disposable.h"

#include "basic.h"
#include "notification.h"

using namespace rx::disposable;

namespace rx::observer {

typedef std::function<void(const notification_t&)> notifier_t;

class Observer : public ObserverBase, public DisposableBase, public std::enable_shared_from_this<Observer> {

public:
    bool is_stopped = false;

private:
    on_next_t _handler_on_next;
    on_error_t _handler_on_error;
    on_completed_t _handler_on_completed;

protected:
    Observer(
        const on_next_t& on_next = DEFAULT_ON_NEXT,
        const on_error_t& on_error = DEFAULT_ON_ERROR,
        const on_completed_t& on_completed = DEFAULT_ON_COMPLETED
    ) : 
        _handler_on_next(on_next), 
        _handler_on_error(on_error), 
        _handler_on_completed(on_completed)
    {}
public:
    inline std::shared_ptr<Observer> getptr() { return shared_from_this(); }
    inline static std::shared_ptr<Observer> get(const on_next_t& on_next = DEFAULT_ON_NEXT, const on_error_t& on_error = DEFAULT_ON_ERROR, const on_completed_t& on_completed = DEFAULT_ON_COMPLETED) { return std::shared_ptr<Observer>(new Observer(on_next, on_error, on_completed)); }
    ~Observer() { this->dispose(); }

    void on_next(const Variant& item) override;
    void on_error(const std::exception& error) override;
    void on_completed() override;

protected:
    virtual void _on_next_core(const Variant& item);
    virtual void _on_error_core(const std::exception& error);
    virtual void _on_completed_core();

public:
    virtual void dispose() override;

    bool fail(const std::exception& error);
    void throw_error(const std::exception& error) const;
    
    notifier_t to_notifier();
    std::shared_ptr<ObserverBase> as_observer();

};

} // END namespace rx::observer

#endif // RX_OBSERVER_OBSERVER_H