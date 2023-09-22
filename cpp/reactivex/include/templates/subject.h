#ifndef RX_TEMPLATES_SUBJECT_H
#define RX_TEMPLATES_SUBJECT_H

#include "wrapper/templates.h"

#include "subject/subject.h"

using namespace rx::wrappers;
using namespace rx::subject;

namespace rx::templates {

class Subject_ : public Subject {

private:
    Ref<RxSubjectTemplate_> self;
protected:
    Subject_(){}
public:
    ~Subject_(){}
    inline subject_ getptr() { return std::static_pointer_cast<Subject_>(Subject::getptr()); }
    inline static subject_ get() { return std::shared_ptr<Subject_>(new Subject_()); }
    inline void _template(Ref<RxSubjectTemplate_> t) { this->self = t; }

    void _on_next_core(const Variant& item) override {
        RX_TEMPLATE_CALL(_on_next_core, self, getptr(), item);
    }
    void _on_error_core(const error_t& error) override {
        RX_TEMPLATE_CALL(_on_error_core, self, getptr(), RxError::wrap(error));
    }
    void _on_completed_core() override {
        RX_TEMPLATE_CALL(_on_completed_core, self, getptr());
    }

    disposable_t _subscribe_core(
        const observer_t& observer, 
        const scheduler_t& scheduler = nullptr
    ) override {
        return RX_TEMPLATE_CALL(_subscribe_core, self, getptr(), observer, scheduler);
    }

    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Subject>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

class SubjectBase_ : public SubjectBase, public std::enable_shared_from_this<SubjectBase_> {

private:
    Ref<RxSubjectTemplate_Base_> self;
protected:
    SubjectBase_(){}
public:
    ~SubjectBase_(){}
    inline subject_base_ getptr() { return this->shared_from_this(); }
    inline static subject_base_ get() { return std::shared_ptr<SubjectBase_>(new SubjectBase_()); }
    inline void _template(Ref<RxSubjectTemplate_Base_> t) { this->self = t; }

    void on_next(const Variant& i) override {
        RX_TEMPLATE_CALL(_on_next, self, getptr(), i);
    }
    void on_error(const error_t& e) override {
        RX_TEMPLATE_CALL(_on_error, self, getptr(), RxError::wrap(e));
    }
    void on_completed() override {
        RX_TEMPLATE_CALL(_on_completed, self, getptr());
    }

    disposable_t subscribe(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed, const scheduler_t& scheduler = nullptr) override {
        return RX_TEMPLATE_CALL(_subscribe, self, getptr(), memnew(RxOnNext(on_next)), memnew(RxOnError(on_error)), memnew(RxOnCompleted(on_completed)), scheduler);
    }
    disposable_t subscribe(const observer_t& observer, const scheduler_t& scheduler = nullptr) override {
        return RX_TEMPLATE_CALL(_subscribe_observer, self, getptr(), observer, scheduler);
    }

    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Subject>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

} // END namespace rx::templates

#endif // RX_TEMPLATES_SUBJECT_H