//
//  ae_db_constants.h
//

#pragma once

#include <string>

namespace yas::ae::db_constants {
namespace module_name {
    static std::string const entity = "module";

    namespace attribute {
        static std::string const file_frame = "file_frame";
        static std::string const range_frame = "range_frame";
        static std::string const range_length = "range_length";
    }  // namespace attribute
}  // namespace module_name

namespace marker_name {
    static std::string const entity = "marker";

    namespace attribute {
        static std::string const frame = "frame";
    }
}  // namespace marker_name
}  // namespace yas::ae::db_constants
