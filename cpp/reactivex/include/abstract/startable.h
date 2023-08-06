#ifndef RX_ABSTRACT_STARTABLE_H
#define RX_ABSTRACT_STARTABLE_H

#include <godot_cpp/variant/variant.hpp>
#include "exception/exception.h"

#include <functional>
#include <memory>

using namespace godot;
using namespace rx::exception;

namespace rx {
namespace abstract {

class StartableBase {

public:
    virtual void start() = 0;
};

} // END namespace abstract

typedef std::function<Variant()> run_t;
typedef std::function<std::shared_ptr<abstract::StartableBase>(const run_t&)> thread_factory_t;

} // END namespace rx

#endif // RX_ABSTRACT_STARTABLE_H