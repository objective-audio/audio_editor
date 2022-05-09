//
//  ae_jumper_dependency.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <optional>

namespace yas::ae {
struct jumpable_on_project_editor {
    virtual ~jumpable_on_project_editor() = default;

    [[nodiscard]] virtual std::optional<frame_index_t> next_jumpable_frame(frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<frame_index_t> previous_jumpable_frame(frame_index_t const) const = 0;
};
}  // namespace yas::ae
