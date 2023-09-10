#include "scheduler/trampoline.h"

namespace rx::scheduler {

bool Trampoline::idle() {
    std::lock_guard<Lock> guard(_lock);
    return this->_idle;
}

void Trampoline::run(const scheduled_item_t& item) {
    {
        std::lock_guard<Lock> guard(_lock);
        this->_queue.push(item);
        if (this->_idle) {
            this->_idle = false;
        }
        else {
            this->_condition.notify_one();
            return;
        }
    }
    try {
        this->_run();
    } 
    catch(...) {}
    {
        std::lock_guard<Lock> guard(_lock);
        this->_idle = true;
        while (!this->_queue.empty()) {
            this->_queue.pop();
        }
    }
}

void Trampoline::_run() {
    std::deque<scheduled_item_t> ready;
    while (true) {
        {
            std::lock_guard<Lock> guard(_lock);
            while (this->_queue.size() > 0) {
                auto item = this->_queue.top();
                if (item.duetime <= item.scheduler->now()) {
                    this->_queue.pop();
                    ready.push_back(item);
                }
                else 
                    break;
            }
        }

        while (ready.size() > 0) {
            auto item = ready.front();
            ready.pop_front();
            if (!item.is_cancelled()) {
                item.invoke();
            }
        }

        {
            std::lock_guard<Lock> guard(_lock);
            if (this->_queue.size() == 0)
                break;
            auto item = this->_queue.top();
            double seconds = (std::chrono::duration<double>(item.duetime - item.scheduler->now())).count();
            if (seconds > 0.0) {
                this->_condition.wait_for(_lock, std::chrono::duration<double>(seconds));
            }
        }
    }
}

} // END namespace rx::scheduler