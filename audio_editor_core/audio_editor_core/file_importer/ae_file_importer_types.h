//
//  ae_file_importer_types.h
//

#pragma once

#include <cpp_utils/yas_url.h>

#include <functional>

namespace yas::ae {
struct file_importing_context {
    std::string identifier;
    url src_url;
    url dst_url;
    std::function<void(bool const)> completion;
};
}  // namespace yas::ae
