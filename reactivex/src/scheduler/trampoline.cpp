#include "trampoline.h"

Ref<Trampoline> Trampoline::Get() {
    auto tramp = memnew(Trampoline);
    tramp->_idle = true;
    tramp->_lock = Lock::Get();
    return tramp;
}

void Trampoline::_bind_methods() {

}

bool Trampoline::idle() {
    std::lock_guard<Lock> guard(**_lock);
    return this->_idle;
}

/*void Trampoline::run(Ref<ScheduledItem> item) {
    {
        std::lock_guard<Lock> guard(*_lock);

    }
}*/