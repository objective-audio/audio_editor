//
//  ae_track_content.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <ui/yas_ui_types.h>

namespace yas::ae {
struct track_content {
    track_index_t identifier;
    bool is_selected;
    ui::size scale;

    track_index_t const &track() const;
    std::string const name() const;

    bool operator==(track_content const &rhs) const;
    bool operator!=(track_content const &rhs) const;
};
}  // namespace yas::ae
