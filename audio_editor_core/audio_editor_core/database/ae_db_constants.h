//
//  ae_db_constants.h
//

#pragma once

#include <string>

namespace yas::ae::db_constants {
namespace module {
    static std::string const entity_name = "module";
    static std::string const file_frame_name = "file_frame";
    static std::string const range_frame_name = "range_frame";
    static std::string const range_length_name = "range_length";
}  // namespace module

namespace marker {
    static std::string const entity_name = "marker";
    static std::string const frame_name = "frame";
}  // namespace marker
}  // namespace yas::ae::db_constants
