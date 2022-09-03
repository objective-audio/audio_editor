//
//  ae_db_marker_constants.h
//

#pragma once

#include <string>

namespace yas::ae::db_constants {
namespace marker_name {
    static std::string const entity = "marker";

    namespace attribute {
        static std::string const frame = "frame";
        static std::string const name = "name";
    }  // namespace attribute
}  // namespace marker_name
}  // namespace yas::ae::db_constants
