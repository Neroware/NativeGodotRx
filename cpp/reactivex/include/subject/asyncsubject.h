#ifndef RX_SUBJECT_ASYNCSUBJECT_H
#define RX_SUBJECT_ASYNCSUBJECT_H

#include <godot_cpp/variant/variant.hpp>

#include "subject/subject.h"

using namespace godot;

namespace rx::subject {

class AsyncSubject : public Subject {

public:
    Variant value;
    bool has_value = false;

private:
    std::shared_ptr<DisposableBase> _subscribe_core(
        const std::shared_ptr<ObserverBase>& observer, 
        const std::shared_ptr<SchedulerBase>& scheduler = nullptr
    ) override;
    void _on_next_core(const Variant& item) override;
    void _on_completed_core() override;

protected:
    AsyncSubject(){}
public:
    ~AsyncSubject(){ this->dispose(); }
    inline static std::shared_ptr<AsyncSubject> get() { return std::shared_ptr<AsyncSubject>(new AsyncSubject()); }
    inline std::shared_ptr<AsyncSubject> getptr() { return std::static_pointer_cast<AsyncSubject>(Subject::getptr()); }

    void dispose() override;

}; // END class AsyncSubject

} // END namespace rx::subject

#endif // RX_SUBJECT_ASYNCSUBJECT_H