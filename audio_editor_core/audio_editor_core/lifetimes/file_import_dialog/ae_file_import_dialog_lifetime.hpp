//
//  ae_file_import_dialog_lifetime.hpp
//

#pragma once

#include <audio_editor_core/ae_project_sub_lifetime_id.h>

namespace yas::ae {
struct file_import_dialog_lifetime final {
    file_import_dialog_lifetime(project_sub_lifetime_id const &);

    project_sub_lifetime_id const lifetime_id;

   private:
    file_import_dialog_lifetime(file_import_dialog_lifetime const &) = delete;
    file_import_dialog_lifetime(file_import_dialog_lifetime &&) = delete;
    file_import_dialog_lifetime &operator=(file_import_dialog_lifetime const &) = delete;
    file_import_dialog_lifetime &operator=(file_import_dialog_lifetime &&) = delete;
};
}  // namespace yas::ae
