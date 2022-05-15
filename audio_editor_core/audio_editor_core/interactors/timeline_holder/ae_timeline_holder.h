//
//  ae_timeline_holder.h
//

#pragma once

#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_url.h>
#include <processing/yas_processing_ptr.h>

namespace yas::ae {
struct timeline_holder final {
    [[nodiscard]] static std::shared_ptr<timeline_holder> make_shared(url const &editing_file_url,
                                                                      project_format const &);

    timeline_holder(url const &editing_file_url, project_format const &);

    proc::timeline_ptr const &timeline() const;

    void replace(file_track_module_map_t const &);
    void insert(file_module const &);
    void erase(time::range const &);

   private:
    url const _editing_file_url;
    project_format const _project_format;

    proc::timeline_ptr const _timeline;
    proc::track_ptr _track;
};
}  // namespace yas::ae
