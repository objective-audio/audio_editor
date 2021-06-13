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
