//
//  action_id.h
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>
#include <cpp-utils/identifier.h>

#include <optional>

namespace yas::ae {
struct action_id final {
    std::optional<identifier> project_instance;
    std::optional<project_guid> project_guid;

    action_id(project_lifetime_id const &);

    // test_utilsにテスト用としてoperator==が定義されている
};
}  // namespace yas::ae

namespace yas {
[[nodiscard]] bool is_match(std::optional<ae::action_id> const &, std::optional<ae::action_id> const &);
}
