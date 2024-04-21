//
//  ae_file_export_dialog_lifetime.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_project_sub_lifetime_id.h>

namespace yas::ae {
struct file_export_dialog_lifetime final {
    file_export_dialog_lifetime(project_sub_lifetime_id const &);

    project_sub_lifetime_id const lifetime_id;

   private:
    file_export_dialog_lifetime(file_export_dialog_lifetime const &) = delete;
    file_export_dialog_lifetime(file_export_dialog_lifetime &&) = delete;
    file_export_dialog_lifetime &operator=(file_export_dialog_lifetime const &) = delete;
    file_export_dialog_lifetime &operator=(file_export_dialog_lifetime &&) = delete;
};
}  // namespace yas::ae
