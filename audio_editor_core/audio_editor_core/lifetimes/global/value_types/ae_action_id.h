//
//  ae_action_id.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_identifier.h>

#include <optional>

namespace yas::ae {
struct action_id final {
    std::optional<identifier> window_instance_id;
    std::optional<project_id> project_id;

    // ae_test_utilsにテスト用としてoperator==が定義されている
};
}  // namespace yas::ae

namespace yas {
[[nodiscard]] bool is_match(std::optional<ae::action_id> const &, std::optional<ae::action_id> const &);
}
