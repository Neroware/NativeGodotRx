#include "wrapper/disposable.h"

#include "disposable/autodisposer.h"

using namespace rx::disposable;

namespace rx::wrappers {

void RxDisposable::dispose() { this->_ptr->dispose(); }
void RxDisposable::dispose_with(Object* obj) { AutoDisposer::add_to(obj, this->_ptr); }

} // END namespace rx::wrappers