//
//  ae_markers_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_marker_pool_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class file_info;

struct project_editor_for_markers_presenter {
    virtual ~project_editor_for_markers_presenter() = default;

    [[nodiscard]] virtual frame_index_t current_frame() const = 0;
    [[nodiscard]] virtual marker_map_t const &markers() const = 0;
    [[nodiscard]] virtual observing::syncable observe_marker_pool_event(
        std::function<void(marker_pool_event const &)> &&) = 0;
};
}  // namespace yas::ae
