//
//  ae_project_sub_level.cpp
//

#include "ae_project_sub_level.h"

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<time_editor_level> const _null_time_editor_level = nullptr;
static std::shared_ptr<sheet_level> const _null_sheet_level = nullptr;
static std::shared_ptr<dialog_level> const _null_dialog_level = nullptr;

std::shared_ptr<time_editor_level> const &yas::get_time_editor_level(
    std::optional<project_sub_level> const &sub_level) {
    if (sub_level.has_value() && std::holds_alternative<std::shared_ptr<time_editor_level>>(sub_level.value())) {
        return std::get<std::shared_ptr<time_editor_level>>(sub_level.value());
    } else {
        return _null_time_editor_level;
    }
}

std::shared_ptr<sheet_level> const &yas::get_sheet_level(std::optional<project_sub_level> const &sub_level) {
    if (sub_level.has_value() && std::holds_alternative<std::shared_ptr<ae::sheet_level>>(sub_level.value())) {
        return std::get<std::shared_ptr<ae::sheet_level>>(sub_level.value());
    } else {
        return _null_sheet_level;
    }
}

std::shared_ptr<dialog_level> const &yas::get_dialog_level(std::optional<project_sub_level> const &sub_level) {
    if (sub_level.has_value() && std::holds_alternative<std::shared_ptr<dialog_level>>(sub_level.value())) {
        return std::get<std::shared_ptr<dialog_level>>(sub_level.value());
    } else {
        return _null_dialog_level;
    }
}
