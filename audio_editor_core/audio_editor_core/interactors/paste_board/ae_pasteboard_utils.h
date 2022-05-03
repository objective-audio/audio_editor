//
//  ae_pasteboard_utils.h
//

#pragma once

#include <map>
#include <string>

namespace yas::ae::pasteboard_utils {
std::string to_data_string(std::map<std::string, std::string> const &);
std::map<std::string, std::string> to_data_map(std::string const &);
}  // namespace yas::ae::pasteboard_utils
