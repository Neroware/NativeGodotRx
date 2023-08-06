#include "wrapper/observer.h"

namespace rx::wrappers {

    void RxObserver::on_next(const Variant& item) {
        this->_ptr->on_next(item);
    }

    void RxObserver::on_error(Ref<RxError> error) {
        this->_ptr->on_error(RxError::unwrap(error));
    }

    void RxObserver::on_completed() {
        this->_ptr->on_completed();
    }

} // END namespace wrappers