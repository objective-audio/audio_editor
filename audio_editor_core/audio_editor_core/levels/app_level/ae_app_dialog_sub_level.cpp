//
//  ae_app_dialog_sub_level.cpp
//

#include "ae_app_dialog_sub_level.h"

using namespace yas;
using namespace yas::ae;

template <typename T>
std::shared_ptr<T> const &yas::get_level(std::optional<ae::app_dialog_sub_level> const &sub_level) {
    if (sub_level.has_value() && std::holds_alternative<std::shared_ptr<T>>(sub_level.value())) {
        return std::get<std::shared_ptr<T>>(sub_level.value());
    } else {
        static std::shared_ptr<T> const _null_level = nullptr;
        return _null_level;
    }
}

template std::shared_ptr<app_dialog_level> const &yas::get_level(std::optional<ae::app_dialog_sub_level> const &);
template std::shared_ptr<project_setup_dialog_level> const &yas::get_level(
    std::optional<ae::app_dialog_sub_level> const &);
