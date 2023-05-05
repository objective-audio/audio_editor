//
//  ae_project_modal_sub_lifetime.cpp
//

#include "ae_project_modal_sub_lifetime.h"

using namespace yas;
using namespace yas::ae;

template <typename T>
std::shared_ptr<T> const &yas::get(std::optional<ae::project_modal_sub_lifetime> const &sub_lifetime) {
    if (sub_lifetime.has_value() && std::holds_alternative<std::shared_ptr<T>>(sub_lifetime.value())) {
        return std::get<std::shared_ptr<T>>(sub_lifetime.value());
    } else {
        static std::shared_ptr<T> const _null_lifetime = nullptr;
        return _null_lifetime;
    }
}

template std::shared_ptr<time_editing_lifetime> const &yas::get(std::optional<ae::project_modal_sub_lifetime> const &);
template std::shared_ptr<module_name_sheet_lifetime> const &yas::get(
    std::optional<ae::project_modal_sub_lifetime> const &);
template std::shared_ptr<marker_name_sheet_lifetime> const &yas::get(
    std::optional<ae::project_modal_sub_lifetime> const &);
template std::shared_ptr<file_import_dialog_lifetime> const &yas::get(
    std::optional<ae::project_modal_sub_lifetime> const &);
template std::shared_ptr<file_export_dialog_lifetime> const &yas::get(
    std::optional<ae::project_modal_sub_lifetime> const &);
template std::shared_ptr<dialog_lifetime> const &yas::get(std::optional<ae::project_modal_sub_lifetime> const &);
template std::shared_ptr<context_menu_lifetime> const &yas::get(std::optional<ae::project_modal_sub_lifetime> const &);

ae::project_modal_sub_lifetime_kind yas::to_kind(std::optional<ae::project_modal_sub_lifetime> const &sub_lifetime) {
    using kind = ae::project_modal_sub_lifetime_kind;

    if (!sub_lifetime.has_value()) {
        return kind::none;
    } else if (std::holds_alternative<std::shared_ptr<time_editing_lifetime>>(sub_lifetime.value())) {
        return kind::time_editor;
    } else if (std::holds_alternative<std::shared_ptr<module_name_sheet_lifetime>>(sub_lifetime.value())) {
        return kind::module_name_sheet;
    } else if (std::holds_alternative<std::shared_ptr<marker_name_sheet_lifetime>>(sub_lifetime.value())) {
        return kind::marker_name_sheet;
    } else if (std::holds_alternative<std::shared_ptr<file_import_dialog_lifetime>>(sub_lifetime.value())) {
        return kind::file_import_dialog;
    } else if (std::holds_alternative<std::shared_ptr<file_export_dialog_lifetime>>(sub_lifetime.value())) {
        return kind::file_export_dialog;
    } else if (std::holds_alternative<std::shared_ptr<dialog_lifetime>>(sub_lifetime.value())) {
        return kind::dialog;
    } else if (std::holds_alternative<std::shared_ptr<context_menu_lifetime>>(sub_lifetime.value())) {
        return kind::context_menu;
    } else {
        throw std::invalid_argument("invalid sub_lifetime");
    }
}
