#include "subject/innersubscription.h"

#include "subject/subject.h"

namespace rx::subject {

void InnerSubscription::dispose() {
    std::lock_guard<RLock> guard(lock);
    if (!this->subject->is_disposed && this->observer) {
        auto elem = std::find(
            this->subject->observers.begin(),
            this->subject->observers.end(),
            this->observer
        );
        if (elem != this->subject->observers.end()) {
            this->subject->observers.erase(elem);
        }
        this->observer = nullptr;
    }
}

} // END namespace rx::subject