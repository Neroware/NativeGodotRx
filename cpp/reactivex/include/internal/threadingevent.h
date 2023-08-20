#ifndef RX_THREADINGEVENT_H
#define RX_THREADINGEVENT_H

#include <condition_variable>
#include <mutex>

namespace rx {

class threading_event {

private:
    std::mutex _mutex;
    std::condition_variable _condition;
    bool _flag = false;

public:
    bool is_set() const;
    void set_flag();
    void clear();
    bool wait(double timeout = -1.0); 

};

} // END namespace rx

#endif // RX_THREADINGEVENT_H