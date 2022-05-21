//
//  ae_project_sub_level.cpp
//

#include "ae_project_sub_level.h"

using namespace yas;
using namespace yas::ae;

template <typename T>
std::shared_ptr<T> const &yas::get_level(std::optional<ae::project_sub_level> const &sub_level) {
    if (sub_level.has_value() && std::holds_alternative<std::shared_ptr<T>>(sub_level.value())) {
        return std::get<std::shared_ptr<T>>(sub_level.value());
    } else {
        static std::shared_ptr<T> const _null_level = nullptr;
        return _null_level;
    }
}

template std::shared_ptr<time_editor_level> const &yas::get_level(std::optional<ae::project_sub_level> const &);
template std::shared_ptr<sheet_level> const &yas::get_level(std::optional<ae::project_sub_level> const &);
template std::shared_ptr<dialog_level> const &yas::get_level(std::optional<ae::project_sub_level> const &);
template std::shared_ptr<context_menu_level> const &yas::get_level(std::optional<ae::project_sub_level> const &);
