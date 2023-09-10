#ifndef RX_SCHEDULER_TRAMPOLINE_H
#define RX_SCHEDULER_TRAMPOLINE_H

#include <deque>
#include <queue>
#include <condition_variable>

#include "internal/time.h"
#include "scheduler/scheduleditem.h"
#include "internal/lock.h"

namespace rx::scheduler {

class Trampoline {

private:
    bool _idle = true;
    std::priority_queue<scheduled_item_t> _queue;
    Lock _lock;
    std::condition_variable_any _condition;

public:
    Trampoline(){}
    ~Trampoline(){}

    bool idle();
    void run(const scheduled_item_t& item);

private:
    void _run();

};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_TRAMPOLINE_H