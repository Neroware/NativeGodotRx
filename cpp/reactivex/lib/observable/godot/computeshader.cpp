#include "observable/definitions.h"

#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

namespace rx::observable {

rx_observable_t godot::from_compute_shader_(const String& shader_path, RenderingDevice* rd, const Vector3i& work_groups, const Array& uniform_sets, const scheduler_t& scheduler) {

    auto loader = ResourceLoader::get_singleton();
    Ref<RDShaderFile> shader_file = loader->load(shader_path);
    auto shader_spirv = shader_file->get_spirv();
    auto shader = rd->shader_create_from_spirv(shader_spirv);
    auto pipeline = rd->compute_pipeline_create(shader);

    Array uniform_sets_rids;
    for (uint64_t sid = 0; sid < uniform_sets.size(); ++sid) {
        uniform_sets_rids.push_back(
            rd->uniform_set_create(uniform_sets[sid], shader, sid)
        );
    }

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton());

        scheduled_action_t action = ACTION(scheduler__, state) {
            auto compute_list = rd->compute_list_begin();
            rd->compute_list_bind_compute_pipeline(compute_list, pipeline);
            for (uint64_t sid = 0; sid < uniform_sets.size(); ++sid) {
                rd->compute_list_bind_uniform_set(compute_list, uniform_sets_rids[sid], sid);
            }
            rd->compute_list_dispatch(compute_list, work_groups.x, work_groups.y, work_groups.z);
			rd->compute_list_end();

            // Send to GPU
            rd->submit();
            // Sync with CPU
            rd->sync();

            observer->on_next(rd);
            observer->on_completed();
            return nullptr;
        };

        return _scheduler->schedule(action);
    };

    return Observable::get(subscribe);
}

} // ENd namespace rx::observable