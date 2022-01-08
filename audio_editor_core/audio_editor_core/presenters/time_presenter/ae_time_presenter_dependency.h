//
//  ae_time_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_file_loader_types.h>

namespace yas::ae {
struct project_editor_for_time_presenter {
    virtual ~project_editor_for_time_presenter() = default;

    [[nodiscard]] virtual ae::file_info const &file_info() const = 0;

    [[nodiscard]] virtual frame_index_t current_frame() const = 0;
};
}  // namespace yas::ae
