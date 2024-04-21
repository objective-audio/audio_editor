//
//  ae_project_modal_sub_lifetime.h
//

#pragma once

#include <ae-core/context_menu/ae_context_menu_lifetime.h>
#include <ae-core/dialog/ae_dialog_lifetime.h>
#include <ae-core/time_editor/ae_time_editing_lifetime.h>

#include <ae-core/file_export_dialog/ae_file_export_dialog_lifetime.hpp>
#include <ae-core/file_import_dialog/ae_file_import_dialog_lifetime.hpp>
#include <ae-core/marker_name_sheet/ae_marker_name_sheet_lifetime.hpp>
#include <ae-core/module_name_sheet/ae_module_name_sheet_lifetime.hpp>
#include <optional>
#include <variant>

namespace yas::ae {
using project_modal_sub_lifetime =
    std::variant<std::shared_ptr<time_editing_lifetime>, std::shared_ptr<module_name_sheet_lifetime>,
                 std::shared_ptr<marker_name_sheet_lifetime>, std::shared_ptr<file_import_dialog_lifetime>,
                 std::shared_ptr<file_export_dialog_lifetime>, std::shared_ptr<dialog_lifetime>,
                 std::shared_ptr<context_menu_lifetime>>;

enum class project_modal_sub_lifetime_kind {
    none,
    time_editing,
    module_name_sheet,
    marker_name_sheet,
    file_import_dialog,
    file_export_dialog,
    dialog,
    context_menu,
};
}  // namespace yas::ae

namespace yas {
template <typename T>
std::shared_ptr<T> const &get(std::optional<ae::project_modal_sub_lifetime> const &);

ae::project_modal_sub_lifetime_kind to_kind(std::optional<ae::project_modal_sub_lifetime> const &);
}  // namespace yas
