//
//  ae_track_content.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_common_types.h>
#include <ui/common/yas_ui_types.h>

namespace yas::ae {
struct track_content {
    track_index_t identifier;
    bool is_selected;
    float scale;

    track_index_t const &track() const;
    std::string const name() const;
    ui::range vertical_range() const;
    float const bottom_y() const;
    float const height() const;

    bool operator==(track_content const &rhs) const;
    bool operator!=(track_content const &rhs) const;
};
}  // namespace yas::ae
