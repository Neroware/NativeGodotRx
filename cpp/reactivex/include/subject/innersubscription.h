#ifndef RX_SUBJECT_INNERSUBSCRIPTION_H
#define RX_SUBJECT_INNERSUBSCRIPTION_H

#include "disposable/disposable.h"
#include "abstract/observer.h"
#include "internal/rlock.h"

using namespace rx::disposable;
using namespace rx::abstract;

namespace rx::subject {

class Subject;

class InnerSubscription : public DisposableBase {

public:
    std::shared_ptr<Subject> subject;
    std::shared_ptr<ObserverBase> observer;
    RLock lock;
    
public:
    InnerSubscription(const std::shared_ptr<Subject>& subject, const std::shared_ptr<ObserverBase>& observer = nullptr) {}
    ~InnerSubscription(){ this->dispose(); }

    void dispose() override;

}; // END class InnerSubscription

} // END namespace rx::subject

#endif // RX_SUBJECT_INNERSUBSCRIPTION_H