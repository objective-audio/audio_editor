//
//  ae_pasteboard_constants.h
//

#pragma once

#include <string>

namespace yas::ae::pasteboard_constants {
namespace file_module_key {
    static std::string const kind = "kind";
    static std::string const name = "name";
    static std::string const file_frame = "file_frame";
    static std::string const frame = "frame";
    static std::string const length = "length";
    static std::string const file_name = "file_name";
}  // namespace file_module_key

namespace file_module_kind {
    static std::string const value = "file_module";
}
}  // namespace yas::ae::pasteboard_constants
