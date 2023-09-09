#ifndef RX_ABSTRACT_SUBJECT_H
#define RX_ABSTRACT_SUBJECT_H

#include "abstract/observer.h"
#include "abstract/observable.h"
#include "typing.h"

using namespace godot;

namespace rx::abstract {

class SubjectBase : public ObservableBase, public ObserverBase {};

}

#endif // RX_ABSTRACT_STARTABLE_H