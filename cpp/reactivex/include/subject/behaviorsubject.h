#ifndef RX_SUBJECT_BEHAVIORSUBJECT_H
#define RX_SUBJECT_BEHAVIORSUBJECT_H

#include <godot_cpp/variant/variant.hpp>

#include "subject/subject.h"

using namespace godot;

namespace rx::subject {

class BehaviorSubject : public Subject {

public:
    Variant value;

private:
    std::shared_ptr<DisposableBase> _subscribe_core(
        const std::shared_ptr<ObserverBase>& observer, 
        const std::shared_ptr<SchedulerBase>& scheduler = nullptr
    ) override;
    void _on_next_core(const Variant& item) override;

protected:
    BehaviorSubject(const Variant& value_) 
        : value(value_) {}
public:
    ~BehaviorSubject(){ this->dispose(); }
    inline static std::shared_ptr<BehaviorSubject> get(const Variant& value_) { return std::shared_ptr<BehaviorSubject>(new BehaviorSubject(value_)); }
    inline std::shared_ptr<BehaviorSubject> getptr() { return std::static_pointer_cast<BehaviorSubject>(Subject::getptr()); }

    void dispose() override;

}; // END class BehaviorSubject

} // END namespace rx::subject

#endif // RX_SUBJECT_BEHAVIORSUBJECT_H