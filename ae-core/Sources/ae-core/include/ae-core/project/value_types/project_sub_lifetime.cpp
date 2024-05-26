//
//  project_sub_lifetime.cpp
//

#include "project_sub_lifetime.h"

using namespace yas;
using namespace yas::ae;

template <typename T>
std::shared_ptr<T> const &yas::get(std::optional<ae::project_sub_lifetime> const &sub_lifetime) {
    if (sub_lifetime.has_value() && std::holds_alternative<std::shared_ptr<T>>(sub_lifetime.value())) {
        return std::get<std::shared_ptr<T>>(sub_lifetime.value());
    } else {
        static std::shared_ptr<T> const _null_lifetime = nullptr;
        return _null_lifetime;
    }
}

template std::shared_ptr<project_launching_lifetime> const &yas::get(std::optional<ae::project_sub_lifetime> const &);
template std::shared_ptr<project_editing_lifetime> const &yas::get(std::optional<ae::project_sub_lifetime> const &);

ae::project_sub_lifetime_kind yas::to_kind(std::optional<ae::project_sub_lifetime> const &sub_lifetime) {
    using kind = ae::project_sub_lifetime_kind;

    if (!sub_lifetime.has_value()) {
        return kind::none;
    } else if (std::holds_alternative<std::shared_ptr<project_launching_lifetime>>(sub_lifetime.value())) {
        return kind::launching;
    } else if (std::holds_alternative<std::shared_ptr<project_editing_lifetime>>(sub_lifetime.value())) {
        return kind::editing;
    } else {
        throw std::invalid_argument("invalid sub_lifetime");
    }
}
