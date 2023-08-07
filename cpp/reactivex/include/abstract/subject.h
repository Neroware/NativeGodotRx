#ifndef RX_ABSTRACT_SUBJECT_H
#define RX_ABSTRACT_SUBJECT_H

#include "abstract/observer.h"
#include "abstract/observable.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::exception;

namespace rx::abstract {

class SubjectBase : public ObservableBase, public ObserverBase {};

}

#endif // RX_ABSTRACT_STARTABLE_H