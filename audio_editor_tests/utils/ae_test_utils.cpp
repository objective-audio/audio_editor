//
//  ae_test_utils.cpp
//

#include "ae_test_utils.h"

#include <cpp_utils/yas_file_manager.h>
#include <cpp_utils/yas_system_path_utils.h>
#include <cpp_utils/yas_url.h>

using namespace yas;
using namespace yas::ae;

url test_utils::test_url() {
    auto const directory = system_path_utils::directory_url(system_path_utils::dir::temporary);
    return directory.appending("jp.objective-audio.audio_editor_mac_tests");
}

void test_utils::create_test_directory() {
    auto result = file_manager::create_directory_if_not_exists(test_url().path());
}

void test_utils::remove_contents_in_test_directory() {
    file_manager::remove_contents_in_directory(test_url().path());
}

bool operator==(yas::ae::action_id const &lhs, yas::ae::action_id const &rhs) {
    return lhs.window_instance_id == rhs.window_instance_id && lhs.project_id == rhs.project_id;
}

bool operator==(std::optional<yas::ae::action_id> const &lhs, std::optional<yas::ae::action_id> const &rhs) {
    if (lhs.has_value() && rhs.has_value()) {
        return *lhs == *rhs;
    } else if (!lhs.has_value() && !rhs.has_value()) {
        return true;
    }
    return false;
}

bool operator==(yas::ae::action const &lhs, yas::ae::action const &rhs) {
    return lhs.kind == rhs.kind && lhs.action_id == rhs.action_id && lhs.value == rhs.value;
}

bool operator==(std::optional<yas::ae::action> const &lhs, std::optional<yas::ae::action> const &rhs) {
    if (lhs.has_value() && rhs.has_value()) {
        return *lhs == *rhs;
    } else if (!lhs.has_value() && !rhs.has_value()) {
        return true;
    }
    return false;
}
