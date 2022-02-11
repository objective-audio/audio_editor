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

namespace edge_name {
    static std::string const entity = "edge";

    namespace attribute {
        static std::string const begin = "begin";
        static std::string const end = "end";
    }  // namespace attribute
}  // namespace edge_name

namespace pasting_subject_name {
    static std::string const entity = "pasting_subject";

    namespace attribute {
        static std::string const data = "data";
    }  // namespace attribute
};     // namespace pasting_subject_name
}  // namespace yas::ae::db_constants
