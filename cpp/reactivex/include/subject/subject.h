#ifndef RX_SUBJECT_SUBJECT_H
#define RX_SUBJECT_SUBJECT_H

#include "abstract/subject.h"
#include "observable/observable.h"
#include "observer/observer.h"

#include "internal/rlock.h"

#include "exception/exception.h"

#include <list>

using namespace rx::observer;
using namespace rx::observable;

namespace rx::subject {

class Subject : public Observable, public Observer, public SubjectBase {

public:
    bool is_disposed = false;
    std::list<std::shared_ptr<ObserverBase>> observers;
    std::unique_ptr<std::exception> exception;
    RLock lock;

protected:
    Subject(){}
public:
    ~Subject(){}
    inline static std::shared_ptr<Subject> get() { return std::shared_ptr<Subject>(new Subject()); }
    inline std::shared_ptr<Subject> getptr() { return std::static_pointer_cast<Subject>(Observer::getptr()); }

    void check_disposed();
    void dispose() override;

    void on_next(const Variant& i) override;
    void on_error(const std::exception& e) override;
    void on_completed() override;

    inline std::shared_ptr<DisposableBase> subscribe(
        const on_next_t& on_next = DEFAULT_ON_NEXT,
        const on_error_t& on_error = DEFAULT_ON_ERROR,
        const on_completed_t& on_completed = DEFAULT_ON_COMPLETED,
        const std::shared_ptr<SchedulerBase>& scheduler = nullptr
    ) { return Observable::subscribe(on_next, on_error, on_completed, scheduler); }
    inline std::shared_ptr<DisposableBase> subscribe(
        const std::shared_ptr<ObserverBase>& observer, 
        const std::shared_ptr<SchedulerBase>& scheduler = nullptr
    ) { return Observable::subscribe(observer, scheduler); };

private:
    void _on_next_core(const Variant& item) override;
    void _on_error_core(const std::exception& error) override;
    void _on_completed_core() override;

    std::shared_ptr<DisposableBase> _subscribe_core(
        const std::shared_ptr<ObserverBase>& observer, 
        const std::shared_ptr<SchedulerBase>& scheduler = nullptr
    ) override;

}; // END class Subject

} // END namespace rx::subject

#endif // RX_SUBJECT_SUBJECT_H