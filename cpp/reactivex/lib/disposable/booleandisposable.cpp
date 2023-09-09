#include "disposable/booleandisposable.h"

namespace rx::disposable {

void BooleanDisposable::dispose() {
    this->is_disposed = true;
}

} // END namespace rx::disposable