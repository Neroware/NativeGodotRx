#include "scheduler/scheduleditem.h"

namespace rx::scheduler {

void scheduled_item_t::invoke() const {
    auto ret = this->scheduler->invoke_action(this->action, this->state);
    this->disposable->set_disposable(ret);
}

void scheduled_item_t::cancel() const {
    this->disposable->dispose();
}

bool scheduled_item_t::is_cancelled() const {
    return this->disposable->is_disposed;
}

bool scheduled_item_t::operator==(const scheduled_item_t& other) const {
    return this->duetime == other.duetime;
}
bool scheduled_item_t::operator!=(const scheduled_item_t& other) const {
    return this->duetime != other.duetime;
}
bool scheduled_item_t::operator<(const scheduled_item_t& other) const {
    return this->duetime < other.duetime;
}
bool scheduled_item_t::operator>(const scheduled_item_t& other) const {
    return this->duetime > other.duetime;
}

} // END namespace scheduler