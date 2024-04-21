//
//  ae_timeline_holder.h
//

#pragma once

#include <ae-core/app/value_types/ae_project_format.h>
#include <ae-core/project_editing/value_types/ae_module_pool_types.h>
#include <audio-processing/common/ptr.h>

namespace yas::ae {
class project_path;

struct timeline_holder final {
    timeline_holder(project_format const &, project_path const *);

    [[nodiscard]] proc::timeline_ptr const &timeline() const;

    void replace(module_pool_module_map_t const &);
    void insert(module const &);
    void erase(track_index_t const, time::range const &);

   private:
    project_format const _project_format;
    project_path const *const _project_path;

    proc::timeline_ptr const _timeline;

    timeline_holder(timeline_holder const &) = delete;
    timeline_holder(timeline_holder &&) = delete;
    timeline_holder &operator=(timeline_holder const &) = delete;
    timeline_holder &operator=(timeline_holder &&) = delete;
};
}  // namespace yas::ae
