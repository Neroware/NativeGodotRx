#ifndef RX_OBSERVER_AUTODETACHOBSERVER_H
#define RX_OBSERVER_AUTODETACHOBSERVER_H

#include "abstract/observer.h"
#include "abstract/disposable.h"

#include "basic.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/disposable.h"

using namespace rx::disposable;

namespace rx::observer {

class AutoDetachObserver : public ObserverBase, public DisposableBase {

private:
    on_next_t _on_next;
    on_error_t _on_error;
    on_completed_t _on_completed;
    SingleAssignmentDisposable _subscription;
public:
    bool is_stopped = false;

public:
    AutoDetachObserver(
        const on_next_t& on_next = DEFAULT_ON_NEXT,
        const on_error_t& on_error = DEFAULT_ON_ERROR,
        const on_completed_t& on_completed = DEFAULT_ON_COMPLETED
    ) : 
        _on_next(on_next), 
        _on_error(on_error), 
        _on_completed(on_completed)
    {}
    ~AutoDetachObserver() { this->dispose(); }

public:
    void on_next(const Variant& item) override;
    void on_error(const std::exception& error) override;
    void on_completed() override;

    void set_disposable(const std::shared_ptr<DisposableBase>& disp);
    void dispose() override;
    bool fail(const std::exception& err);
};

} // END namespace rx::observer

#endif // RX_OBSERVER_AUTODETACHOBSERVER_H