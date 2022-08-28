//
//  ae_marker_name_sheet_lifetime.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_project_sub_lifetime_id.h>

#include <memory>

namespace yas::ae {
class marker_name_editor;

struct marker_name_sheet_lifetime final {
    marker_name_sheet_lifetime(project_sub_lifetime_id const &, int64_t const marker_frame);

    project_sub_lifetime_id const lifetime_id;
    std::shared_ptr<marker_name_editor> const editor;
};
}  // namespace yas::ae
