//
//  ae_timeline_processor.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <cpp_utils/yas_url.h>
#include <processing/yas_processing_ptr.h>

namespace yas::ae {
struct timeline_processor final {
    [[nodiscard]] static std::shared_ptr<timeline_processor> make_shared(std::string const &project_id,
                                                                         ae::file_info const &);
    [[nodiscard]] static std::shared_ptr<timeline_processor> make_shared(url const &editing_file_url,
                                                                         ae::file_info const &);

    proc::timeline_ptr const &timeline() const;

    void replace(file_track_module_map_t const &);
    void insert(file_module const &);
    void erase(time::range const &);

   private:
    url const _editing_file_url;
    ae::file_info const _file_info;

    proc::timeline_ptr const _timeline;
    proc::track_ptr _track;

    timeline_processor(url const &editing_file_url, ae::file_info const &);
};
}  // namespace yas::ae
