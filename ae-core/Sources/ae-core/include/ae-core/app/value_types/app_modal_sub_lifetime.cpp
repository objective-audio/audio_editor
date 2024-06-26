//
//  app_modal_sub_lifetime.cpp
//

#include "app_modal_sub_lifetime.h"

using namespace yas;
using namespace yas::ae;

template <typename T>
std::shared_ptr<T> const &yas::get(std::optional<ae::app_modal_sub_lifetime> const &sub_lifetime) {
    if (sub_lifetime.has_value() && std::holds_alternative<std::shared_ptr<T>>(sub_lifetime.value())) {
        return std::get<std::shared_ptr<T>>(sub_lifetime.value());
    } else {
        static std::shared_ptr<T> const _null_lifetime = nullptr;
        return _null_lifetime;
    }
}

template std::shared_ptr<project_setup_dialog_lifetime> const &yas::get(
    std::optional<ae::app_modal_sub_lifetime> const &);

app_modal_sub_lifetime_kind yas::to_kind(std::optional<ae::app_modal_sub_lifetime> const &sub_lifetime) {
    using kind = app_modal_sub_lifetime_kind;

    if (!sub_lifetime.has_value()) {
        return kind::none;
    } else if (std::holds_alternative<std::shared_ptr<project_setup_dialog_lifetime>>(sub_lifetime.value())) {
        return kind::project_setup_dialog;
    } else {
        throw std::invalid_argument("invalid sub_lifetime");
    }
}
