#include "scheduleditem.h"

ScheduledItem::ScheduledItem(Ref<Scheduler> scheduler, const Variant& state, const Callable& action, Ref<AbsoluteTime> duetime) : scheduler(scheduler), state(state), action(action), duetime(duetime), disposable(SingleAssignmentDisposable::Get()) {}

void ScheduledItem::invoke() {
    auto ret = this->scheduler->invoke_action(this->action, this->state);
    this->disposable->set_disposable(ret);
}

void ScheduledItem::cancel() {
    this->disposable->dispose();
}

bool ScheduledItem::is_cancelled() {
    return this->disposable->is_disposed;
}

bool ScheduledItem::operator==(const ScheduledItem& other) {
    return *(this->duetime) == *(other.duetime);
}
bool ScheduledItem::operator<(const ScheduledItem& other) {
    return *(this->duetime) < *(other.duetime);
}
bool ScheduledItem::operator>(const ScheduledItem& other) {
    return *(this->duetime) > *(other.duetime);
}