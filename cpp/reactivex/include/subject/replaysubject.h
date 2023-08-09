#ifndef RX_SUBJECT_REPLAYSUBJECT_H
#define RX_SUBJECT_REPLAYSUBJECT_H

#include <godot_cpp/variant/variant.hpp>

#include "subject/subject.h"
#include "scheduler/currentthreadscheduler.h"

#include <deque>

using namespace godot;
using namespace rx::scheduler;

namespace rx::subject {

class RemovableDisposable : public DisposableBase {
public:
    std::shared_ptr<Subject> subject;
    std::shared_ptr<Observer> observer;

    RemovableDisposable(const std::shared_ptr<Subject>& subject_, const std::shared_ptr<Observer>& observer_) 
        : subject(subject_), observer(observer_) {}
    ~RemovableDisposable() { this->dispose(); }

    inline void dispose() override {
        this->observer->dispose();
        auto elem = std::find(this->subject->observers.begin(), this->subject->observers.end(), this->observer);
        if (!this->subject->is_disposed && elem != this->subject->observers.end()) {
            this->subject->observers.erase(elem);
        }
    }
}; // END class RemovableDisposable

struct queue_item_t {
    time_point_t interval;
    Variant value;
}; // END struct queue_item_t

class ReplaySubject : public Subject {

public:
    uint64_t buffer_size;
    std::shared_ptr<SchedulerBase> scheduler;
    time_delta_t window;
    std::deque<queue_item_t> queue;

private:
    std::shared_ptr<DisposableBase> _subscribe_core(
        const std::shared_ptr<ObserverBase>& observer, 
        const std::shared_ptr<SchedulerBase>& scheduler = nullptr
    ) override;
    void _on_next_core(const Variant& value) override;
    void _on_error_core(const std::exception_ptr& error) override;
    void _on_completed_core() override;
    void _trim(const time_point_t& now);

protected:
    ReplaySubject(uint64_t buffer_size_ = 1024ul, const time_delta_t& window_ = std::chrono::milliseconds::max(), const std::shared_ptr<SchedulerBase>& scheduler_ = CurrentThreadScheduler::singleton())
        : buffer_size(buffer_size_), window(window_), scheduler(scheduler_) {}
public:
    ~ReplaySubject(){ this->dispose(); }
    inline static std::shared_ptr<ReplaySubject> get(uint64_t buffer_size_ = 1024ul, const time_delta_t& window_ = std::chrono::milliseconds::max(), const std::shared_ptr<SchedulerBase>& scheduler_ = CurrentThreadScheduler::singleton()) { return std::shared_ptr<ReplaySubject>(new ReplaySubject(buffer_size_, window_, scheduler_)); }
    inline std::shared_ptr<ReplaySubject> getptr() { return std::static_pointer_cast<ReplaySubject>(Subject::getptr()); }

    void dispose() override;

}; // END class ReplaySubject

} // END namespace rx::subject

#endif // RX_SUBJECT_REPLAYSUBJECT_H