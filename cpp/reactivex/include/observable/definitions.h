#ifndef RX_OBSERVABLE_DEFINITIONS_H
#define RX_OBSERVABLE_DEFINITIONS_H

#include "observable/observable.h"

#include "internal/mapping.h"

namespace rx {

namespace observable {

struct obs {
    // catch.h
    static rx_observable_t catch_with_iterable_(const rx_observable_list_t& sources);
    static rx_observable_t catch_with_iterable_(const rx_observable_t& sources...);
    // case.h
    static rx_observable_t case_(const mapper_t<Variant>& mapper, const default_mapping<Variant, rx_observable_t>& sources, const rx_observable_t& default_source = nullptr);
    // combinelatest.h
    static rx_observable_t combine_latest_(const rx_observable_list_t& sources);
    static rx_observable_t combine_latest_(const rx_observable_t& sources...);
    // concat.h
    static rx_observable_t concat_with_iterable_(const rx_observable_list_t& sources);
    static rx_observable_t concat_with_iterable_(const rx_observable_t& sources...);
    // defer.h
    static rx_observable_t defer_(const observable_factory_t& factory);
    // empty.h
    static rx_observable_t empty_(const scheduler_t& scheduler = nullptr);
    // forkjoin.h
    static rx_observable_t fork_join_(const rx_observable_list_t& sources);
    static rx_observable_t fork_join_(const rx_observable_t& sources...);
    // fromiterable.h
    static rx_observable_t from_iterable_(const iterable_t& iterable, const scheduler_t& scheduler = nullptr);
    // generate.h
    static rx_observable_t generate_(const Variant& initial_state, const predicate_t<Variant>& condition, const mapper_t<Variant, Variant>& iterate);
    // ifthen.h
    static rx_observable_t if_then_(const predicate_t<>& condition, const rx_observable_t& then_source, const rx_observable_t& else_source = nullptr);
    // merge.h
    static rx_observable_t merge_(const rx_observable_list_t& sources);
    static rx_observable_t merge_(const rx_observable_t& sources...);
    // never.h
    static rx_observable_t never_();
    // onerrorresumenext.h
    static rx_observable_t on_error_resume_next_(const rx_observable_list_t& sources);
    static rx_observable_t on_error_resume_next_(const rx_observable_t& sources...);
    static rx_observable_t on_error_resume_next_(const RxList<stated_observable_factory_t>& sources);
    static rx_observable_t on_error_resume_next_(const stated_observable_factory_t& sources...);
    // range.h
    static rx_observable_t range_(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1, const scheduler_t& scheduler = nullptr);
    // returnvalue.h
    static rx_observable_t return_value_(const Variant& value, const scheduler_t& scheduler = nullptr);
    // throw.h
    static rx_observable_t throw_(const error_t& error, const scheduler_t& scheduler = nullptr);
    // timer.h
    static rx_observable_t timer_(const time_point_t& duetime, const scheduler_t& scheduler = nullptr);
    static rx_observable_t timer_(const time_point_t& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr);
    static rx_observable_t timer_(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr);
    static rx_observable_t timer_(const time_delta_t& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr);
    static rx_observable_t timer_(const double& duetime, const scheduler_t& scheduler = nullptr);
    static rx_observable_t timer_(const double& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr);
    // using.h
    static rx_observable_t using_(const std::function<disposable_t()>& resource_factory, const std::function<rx_observable_t(const disposable_t&)>& observable_factory);
    // withlatestfrom.h
    static rx_observable_t with_latest_from_(const rx_observable_t& parent, const rx_observable_list_t& sources);
    static rx_observable_t with_latest_from_(const rx_observable_t& parent, const rx_observable_t& sources...);
    // zip.h
    static rx_observable_t zip_(const rx_observable_list_t& sources);
    static rx_observable_t zip_(const rx_observable_t& sources...);
}; // END struct obs

// catch.h
static rx_observable_t catch_with_iterable(const rx_observable_list_t& sources) { return obs::catch_with_iterable_(sources); }
static rx_observable_t catch_with_iterable(const rx_observable_t& sources...) { return obs::catch_with_iterable_(sources); }
// case.h
static rx_observable_t case_mapper(const mapper_t<Variant>& mapper, const default_mapping<Variant, rx_observable_t>& sources, const rx_observable_t& default_source = nullptr) { return obs::case_(mapper, sources, default_source); }
// combinelatest.h
static rx_observable_t combine_latest(const rx_observable_list_t& sources) { return obs::combine_latest_(sources); }
static rx_observable_t combine_latest(const rx_observable_t& sources...) { return obs::combine_latest_(sources); }
// concat.h
static rx_observable_t concat_with_iterable(const rx_observable_list_t& sources) { return obs::concat_with_iterable_(sources); }
static rx_observable_t concat_with_iterable(const rx_observable_t& sources...) { return obs::concat_with_iterable_(sources); }
// defer.h
static rx_observable_t defer(const observable_factory_t& factory) { return obs::defer_(factory); }
// empty.h
static rx_observable_t empty(const scheduler_t& scheduler = nullptr) { return obs::empty_(scheduler); }
// forkjoin.h
static rx_observable_t fork_join(const rx_observable_list_t& sources) { return obs::fork_join_(sources); }
static rx_observable_t fork_join(const rx_observable_t& sources...) { return obs::fork_join_(sources); }
// fromiterable.h
static rx_observable_t from_iterable(const iterable_t& iterable, const scheduler_t& scheduler = nullptr) { return obs::from_iterable_(iterable, scheduler); }
static rx_observable_t from(const iterable_t& iterable, const scheduler_t& scheduler = nullptr) { return obs::from_iterable_(iterable, scheduler); }
// generate.h
static rx_observable_t generate(const Variant& initial_state, const predicate_t<Variant>& condition, const mapper_t<Variant, Variant>& iterate) { return obs::generate_(initial_state, condition, iterate); }
// ifthen.h
static rx_observable_t if_then(const predicate_t<>& condition, const rx_observable_t& then_source, const rx_observable_t& else_source = nullptr) { return obs::if_then_(condition, then_source, else_source); }
// merge.h
static rx_observable_t merge(const rx_observable_list_t& sources) { return obs::merge_(sources); }
static rx_observable_t merge(const rx_observable_t& sources...) { return obs::merge_(sources); }
// never.h
static rx_observable_t never() { return obs::never_(); }
// onerrorresumenext.h
static rx_observable_t on_error_resume_next(const rx_observable_list_t& sources) { return obs::on_error_resume_next_(sources); }
static rx_observable_t on_error_resume_next(const rx_observable_t& sources...) { return obs::on_error_resume_next_(sources); }
static rx_observable_t on_error_resume_next(const RxList<stated_observable_factory_t>& sources) { return obs::on_error_resume_next_(sources); }
static rx_observable_t on_error_resume_next(const stated_observable_factory_t& sources...) { return obs::on_error_resume_next_(sources); }
// range.h
static rx_observable_t range(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1, const scheduler_t& scheduler = nullptr) { return obs::range_(start, stop, step, scheduler); }
// returnvalue.h
static rx_observable_t return_value(const Variant& value, const scheduler_t& scheduler = nullptr) { return obs::return_value_(value, scheduler); }
static rx_observable_t just(const Variant& value, const scheduler_t& scheduler = nullptr) { return obs::return_value_(value, scheduler); }
// throw.h
static rx_observable_t throw_error(const error_t& error, const scheduler_t& scheduler = nullptr) { return obs::throw_(error); }
// timer.h
static rx_observable_t timer(const time_point_t& duetime, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, scheduler); }
static rx_observable_t timer(const time_point_t& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, period, scheduler); }
static rx_observable_t timer(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, scheduler); }
static rx_observable_t timer(const time_delta_t& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, period, scheduler); }
static rx_observable_t timer(const double& duetime, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, scheduler); }
static rx_observable_t timer(const double& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, period, scheduler); }
// using.h
static rx_observable_t using_resource(const std::function<disposable_t()>& resource_factory, const std::function<rx_observable_t(const disposable_t&)>& observable_factory) { return obs::using_(resource_factory, observable_factory); }
// withlatestfrom.h
static rx_observable_t with_latest_from(const rx_observable_t& parent, const rx_observable_list_t& sources) { return obs::with_latest_from_(parent, sources); }
static rx_observable_t with_latest_from(const rx_observable_t& parent, const rx_observable_t& sources...) { return obs::with_latest_from_(parent, sources); }
// zip.h
static rx_observable_t zip(const rx_observable_list_t& sources) { return obs::zip_(sources); }
static rx_observable_t zip(const rx_observable_t& sources...) { return obs::zip_(sources); }

namespace op {

struct ops {
    // _all.h
    static observable_op_t all_(const predicate_t<Variant>& predicate);
    // _amb.h
    static observable_op_t amb_(const rx_observable_t& right_source);
    // _asobservable.h
    static observable_op_t as_observable_();
    // _average.h
    static observable_op_t average_(const mapper_t<double, Variant>& key_mapper = nullptr);
    // _catch.h
    static observable_op_t catch_(const rx_observable_t& handler);
    static observable_op_t catch_(const std::function<rx_observable_t(const error_t&, const rx_observable_t&)>& handler);
    // _combinelatest.h
    static observable_op_t combine_latest_(const rx_observable_list_t& others);
    static observable_op_t combine_latest_(const rx_observable_t& others...);
    // _filter.h
    static observable_op_t filter_(const predicate_t<Variant>& predicate);
    static observable_op_t filter_indexed_(const predicate_indexed_t<Variant>& predicate = nullptr);
    // _last.h
    static observable_op_t last_(const predicate_t<Variant>& predicate = nullptr);
    // _lastordefault.h
    static rx_observable_t last_or_default_async_(const rx_observable_t& source, bool has_default = false, const Variant& default_value = VNULL);
    static observable_op_t last_or_default_(const Variant& default_value = VNULL, const predicate_t<Variant>& predicate = nullptr);
    // _map.h
    static observable_op_t map_(const mapper_t<Variant, Variant>& mapper = nullptr);
    static observable_op_t map_indexed_(const mapper_indexed_t<Variant, Variant>& mapper = nullptr);
    // _materialize.h
    static observable_op_t materialize_();
    // _merge.h
    static observable_op_t merge_(const rx_observable_list_t& sources, int64_t max_concurrent = -1);
    static observable_op_t merge_(int64_t max_concurrent, const rx_observable_t& sources...);
    static observable_op_t merge_all_();
    // _scan.h
    static observable_op_t scan_(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value());
    // _some.h
    static observable_op_t some_(const predicate_t<Variant>& predicate = nullptr);
    // _zip.h
    static observable_op_t zip_(const rx_observable_list_t& sources);
    static observable_op_t zip_(const rx_observable_t& others...);
    static observable_op_t zip_with_iterable_(const iterable_t& seq);

}; // END struct ops

// _all.h
static observable_op_t all(const predicate_t<Variant>& predicate) { return ops::all_(predicate); }
// _amb.h
static observable_op_t amb(const rx_observable_t& right_source) { return ops::amb_(right_source); }
// _asobservable.h
static observable_op_t as_observable() { return ops::as_observable_(); }
// _average.h
static observable_op_t average(const mapper_t<double, Variant>& key_mapper = nullptr) { return ops::average_(key_mapper); }
// _catch.h
static observable_op_t catch_with(const rx_observable_t& handler) { return ops::catch_(handler); }
static observable_op_t catch_with(const std::function<rx_observable_t(const error_t&, const rx_observable_t&)>& handler) { return ops::catch_(handler); }
// _combinelatest.h
static observable_op_t combine_latest(const rx_observable_list_t& others) { return ops::combine_latest_(others); }
static observable_op_t combine_latest(const rx_observable_t& others...) { return ops::combine_latest_(others); }
// _filter.h
static observable_op_t filter(const predicate_t<Variant>& predicate) { return ops::filter_(predicate); }
static observable_op_t filter_indexed(const predicate_indexed_t<Variant>& predicate = nullptr) { return ops::filter_indexed_(predicate); }
// _last.h
static observable_op_t last(const predicate_t<Variant>& predicate = nullptr) { return ops::last_(predicate); }
// _lastordefault.h
static rx_observable_t last_or_default_async(const rx_observable_t& source, bool has_default = false, const Variant& default_value = VNULL) { return ops::last_or_default_async_(source, has_default, default_value); }
static observable_op_t last_or_default(const Variant& default_value = VNULL, const predicate_t<Variant>& predicate = nullptr) { return ops::last_or_default_(default_value, predicate); }
// _map.h
static observable_op_t map(const mapper_t<Variant, Variant>& mapper = nullptr) { return ops::map_(mapper); }
static observable_op_t map_indexed(const mapper_indexed_t<Variant, Variant>& mapper = nullptr) { return ops::map_indexed_(mapper); }
// _materialize.h
static observable_op_t materialize() { return ops::materialize_(); }
// _merge.h
static observable_op_t merge(const rx_observable_list_t& sources, int64_t max_concurrent = -1) { return ops::merge_(sources, max_concurrent); }
static observable_op_t merge(int64_t max_concurrent, const rx_observable_t& sources...) { return ops::merge_(max_concurrent, sources); }
static observable_op_t merge_all() { return ops::merge_all_(); }
// _scan.h
static observable_op_t scan(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value()) { return ops::scan_(accumulator, seed); }
// _some.h
static observable_op_t some(const predicate_t<Variant>& predicate = nullptr) { return ops::some_(predicate); }
// _zip.h
static observable_op_t zip(const rx_observable_list_t& sources) { return ops::zip_(sources); }
static observable_op_t zip(const rx_observable_t& others...) { return ops::zip_(others); }
static observable_op_t zip_with_iterable(const iterable_t& seq) { return ops::zip_with_iterable_(seq); }

} // END namespace op
} // END namespace observable
} // END namespace rx::observable

#endif // RX_OBSERVABLE_DEFINITIONS_H