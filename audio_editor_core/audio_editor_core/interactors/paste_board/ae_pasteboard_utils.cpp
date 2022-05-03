//
//  ae_pasteboard_utils.cpp
//

#include "ae_pasteboard_utils.h"

#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::string pasteboard_utils::to_data_string(std::map<std::string, std::string> const &map) {
    std::vector<std::string> strings;
    for (auto const &pair : map) {
        strings.emplace_back(pair.first + "=" + pair.second);
    }
    return joined(strings, ",");
}

std::map<std::string, std::string> pasteboard_utils::to_data_map(std::string const &data) {
    std::map<std::string, std::string> map;
    for (auto const &pair_string : split(data, ',')) {
        auto const pair = split(pair_string, '=');
        if (pair.size() == 2) {
            map.emplace(pair.at(0), pair.at(1));
        }
    }
    return map;
}
