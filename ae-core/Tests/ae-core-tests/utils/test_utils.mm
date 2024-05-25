//
//  test_utils.cpp
//

#include "test_utils.h"

#import <Foundation/Foundation.h>
#include <cpp-utils/file_manager.h>
#include <cpp-utils/system_path_utils.h>

using namespace yas;
using namespace yas::ae;

std::filesystem::path test_utils::test_path() {
    return system_path_utils::directory_path(system_path_utils::dir::temporary)
        .append("jp.objective-audio.audio_editor_mac_tests");
}

void test_utils::create_test_directory() {
    auto result = file_manager::create_directory_if_not_exists(test_path());
}

void test_utils::remove_contents_in_test_directory() {
    file_manager::remove_contents_in_directory(test_path());
}

void test_utils::clear_user_defaults() {
    NSString *bundleId = NSBundle.mainBundle.bundleIdentifier;
    [NSUserDefaults.standardUserDefaults removePersistentDomainForName:bundleId];
}

bool operator==(yas::ae::action_id const &lhs, yas::ae::action_id const &rhs) {
    return lhs.project_instance == rhs.project_instance && lhs.project_guid == rhs.project_guid;
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
    return lhs.name == rhs.name && lhs.value == rhs.value;
}

bool operator==(std::optional<yas::ae::action> const &lhs, std::optional<yas::ae::action> const &rhs) {
    if (lhs.has_value() && rhs.has_value()) {
        return *lhs == *rhs;
    } else if (!lhs.has_value() && !rhs.has_value()) {
        return true;
    }
    return false;
}
