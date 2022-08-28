//
//  ae_marker_name_sheet_lifetime.cpp
//

#include "ae_marker_name_sheet_lifetime.hpp"

#include <audio_editor_core/ae_marker_name_editor.hpp>

using namespace yas;
using namespace yas::ae;

marker_name_sheet_lifetime::marker_name_sheet_lifetime(project_sub_lifetime_id const &lifetime_id,
                                                       int64_t const marker_frame)
    : lifetime_id(lifetime_id), editor(marker_name_editor::make_shared(lifetime_id, marker_frame)) {
}
