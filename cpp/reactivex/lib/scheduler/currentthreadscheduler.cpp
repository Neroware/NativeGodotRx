#include "scheduler/currentthreadscheduler.h"

#include "godotrx.h"

namespace rx::scheduler {

std::shared_ptr<CurrentThreadScheduler> CurrentThreadScheduler::singleton() {
    auto thread = std::make_shared<variant_key_t>(RxThread::get_current_thread());
    auto& class_map = GDRX->CurrentThreadScheduler_global_[cls];
    
    const auto& class_map_ = class_map;
    if (class_map_.contains(thread)) {
        return class_map_[thread];
    }
    auto res = CurrentThreadSchedulerSingleton::get();
    class_map.insert(thread, res);
    return res;
}

std::shared_ptr<Trampoline> CurrentThreadScheduler::get_trampoline() {
    auto thread = std::make_shared<variant_key_t>(RxThread::get_current_thread());
    const auto& tramps = this->_tramps;
    if (tramps.contains(thread)) {
        return tramps[thread];
    }
    auto tramp = std::make_shared<Trampoline>();
    this->_tramps.insert(thread, tramp);
    return tramp;
}

std::shared_ptr<Trampoline> _CurrentThreadScheduler_Local::get_trampoline() {
    auto thread = std::make_shared<variant_key_t>(RxThread::get_current_thread());
    const auto& tramps = this->_tramp;
    if (tramps.contains(thread)) {
        return tramps[thread];
    }
    auto tramp = std::make_shared<Trampoline>();
    this->_tramp.insert(thread, tramp);
    return tramp;
}

std::shared_ptr<Trampoline> CurrentThreadSchedulerSingleton::get_trampoline() {
    return GDRX->CurrentThreadScheduler_local_->get_trampoline();
}

} // END namespace rx::scheduler