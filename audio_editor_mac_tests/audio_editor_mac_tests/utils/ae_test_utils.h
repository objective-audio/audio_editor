//
//  ae_test_utils.h
//

#pragma once

#include <cpp_utils/yas_url.h>

namespace yas::ae::test_utils {
url test_url();
void create_test_directory();
void remove_contents_in_test_directory();
}  // namespace yas::ae::test_utils
