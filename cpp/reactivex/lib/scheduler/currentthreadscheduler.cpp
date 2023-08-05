#include "scheduler/currentthreadscheduler.h"

#include "godotrx.h"

namespace rx::scheduler {

std::shared_ptr<CurrentThreadScheduler> CurrentThreadScheduler::singleton() {
    auto cls = std::make_shared<variant_key_t>("CurrentThreadScheduler");
    // auto& global = GDRX->CurrentThreadScheduler_global_;

    auto thread = std::make_shared<variant_key_t>(RxThread::get_current_thread());
    auto& class_map = GDRX->CurrentThreadScheduler_global_[cls];
    
    const auto& class_map_ = class_map;
    try {
        return class_map_[thread];
    }
    catch(std::out_of_range) {
        auto res = CurrentThreadSchedulerSingleton::get();
        class_map[thread] = res;
        return res;
    }
}

std::shared_ptr<Trampoline> CurrentThreadScheduler::get_trampoline() {
    auto thread = std::make_shared<variant_key_t>(RxThread::get_current_thread());
    const auto& tramps = this->_tramps;
    try {
        return tramps[thread];
    }
    catch (std::out_of_range) {
        auto tramp = std::make_shared<Trampoline>();
        this->_tramps[thread] = tramp;
        return tramp;
    }
}

std::shared_ptr<Trampoline> _CurrentThreadScheduler_Local::get_trampoline() {
    auto thread = std::make_shared<variant_key_t>(RxThread::get_current_thread());
    const auto& tramps = this->_tramp;
    try {
        return tramps[thread];
    }
    catch (std::out_of_range) {
        auto tramp = std::make_shared<Trampoline>();
        this->_tramp[thread] = tramp;
        return tramp;
    }
}

std::shared_ptr<Trampoline> CurrentThreadSchedulerSingleton::get_trampoline() {
    return GDRX->CurrentThreadScheduler_local_->get_trampoline();
}

} // END namespace rx::scheduler