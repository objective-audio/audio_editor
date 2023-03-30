//
//  ae_test_utils.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_action_id.h>

#include <filesystem>
#include <optional>

namespace yas::ae::test_utils {
std::filesystem::path test_path();
void create_test_directory();
void remove_contents_in_test_directory();
void clear_user_defaults();
}  // namespace yas::ae::test_utils

bool operator==(yas::ae::action_id const &, yas::ae::action_id const &);
bool operator==(std::optional<yas::ae::action_id> const &, std::optional<yas::ae::action_id> const &);
bool operator==(yas::ae::action const &, yas::ae::action const &);
bool operator==(std::optional<yas::ae::action> const &, std::optional<yas::ae::action> const &);
