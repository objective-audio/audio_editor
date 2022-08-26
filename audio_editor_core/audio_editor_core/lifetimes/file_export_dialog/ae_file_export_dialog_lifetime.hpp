//
//  ae_file_export_dialog_lifetime.hpp
//

#pragma once

#include <audio_editor_core/ae_project_sub_lifetime_id.h>

namespace yas::ae {
struct file_export_dialog_lifetime final {
    file_export_dialog_lifetime(dialog_lifetime_id const &);

    dialog_lifetime_id const lifetime_id;
};
}  // namespace yas::ae
