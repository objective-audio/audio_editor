//
//  project_modal_sub_lifetime.h
//

#pragma once

#include <ae-core/context_menu/context_menu_lifetime.h>
#include <ae-core/dialog/dialog_lifetime.h>
#include <ae-core/time_editor/time_editing_lifetime.h>

#include <ae-core/file_export_dialog/file_export_dialog_lifetime.hpp>
#include <ae-core/file_import_dialog/file_import_dialog_lifetime.hpp>
#include <ae-core/marker_name_sheet/marker_name_sheet_lifetime.hpp>
#include <ae-core/module_name_sheet/module_name_sheet_lifetime.hpp>
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
