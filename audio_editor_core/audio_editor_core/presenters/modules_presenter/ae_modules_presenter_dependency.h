//
//  ae_modules_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_file_track_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class file_info;

struct project_editor_for_modules_presenter {
    virtual ~project_editor_for_modules_presenter() = default;

    [[nodiscard]] virtual ae::file_info const &file_info() const = 0;
    [[nodiscard]] virtual frame_index_t current_frame() const = 0;
    [[nodiscard]] virtual file_track_module_map_t const &modules() const = 0;
    [[nodiscard]] virtual observing::syncable observe_file_track_event(
        std::function<void(file_track_event const &)> &&) = 0;
};
}  // namespace yas::ae
