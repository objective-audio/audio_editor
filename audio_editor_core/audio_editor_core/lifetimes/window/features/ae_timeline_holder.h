//
//  ae_timeline_holder.h
//

#pragma once

#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_project_format.h>
#include <processing/yas_processing_ptr.h>

namespace yas::ae {
class project_path;

struct timeline_holder final {
    [[nodiscard]] static std::shared_ptr<timeline_holder> make_shared(project_format const &, project_path const *);

    timeline_holder(project_format const &, project_path const *);

    proc::timeline_ptr const &timeline() const;

    void replace(file_track_module_map_t const &);
    void insert(file_module const &);
    void erase(time::range const &);

   private:
    project_format const _project_format;
    project_path const *const _project_path;

    proc::timeline_ptr const _timeline;
    proc::track_ptr _track;
};
}  // namespace yas::ae
