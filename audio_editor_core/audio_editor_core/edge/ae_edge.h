//
//  ae_edge.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <string>

namespace yas::ae {
struct edge final {
    frame_index_t begin_frame;
    frame_index_t end_frame;

    bool operator==(edge const &) const;
    bool operator!=(edge const &) const;

    [[nodiscard]] static edge const &zero();
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::edge const &);
}

std::ostream &operator<<(std::ostream &, yas::ae::edge const &);
