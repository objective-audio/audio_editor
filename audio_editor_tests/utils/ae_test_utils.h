//
//  ae_test_utils.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <cpp_utils/yas_url.h>

#include <optional>

namespace yas::ae::test_utils {
url test_url();
void create_test_directory();
void remove_contents_in_test_directory();
}  // namespace yas::ae::test_utils

bool operator==(yas::ae::action const &lhs, yas::ae::action const &rhs);
bool operator==(std::optional<yas::ae::action> const &lhs, std::optional<yas::ae::action> const &rhs);
