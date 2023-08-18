#ifndef RX_OBSERVABLE_CASE_H
#define RX_OBSERVABLE_CASE_H

#include "observable/observable.h"
#include "observable/empty.h"
#include "observable/defer.h"
#include "typing.h"

namespace rx::observable {

template<typename KeyT, typename MappingT>
static std::shared_ptr<Observable> case_(
    const mapper_t<KeyT>& mapper,
    const MappingT& sources,
    const std::shared_ptr<Observable>& default_source = nullptr
) {

    auto default_source_ = default_source ? default_source : empty_();

    observable_factory_t factory = [=](const std::shared_ptr<SchedulerBase>& _) -> std::shared_ptr<Observable> {
        KeyT key = mapper();
        std::shared_ptr<Observable> result;
        if (sources.contains(key)) {
            result = sources[key];
        }
        else {
            result = default_source_;
        }

        return result;
    };

    return defer_(factory);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_CASE_H