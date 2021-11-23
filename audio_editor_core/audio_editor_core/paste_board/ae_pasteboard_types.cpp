//
//  ae_pasteboard_types.cpp
//

#include "ae_pasteboard_types.h"

#include <audio_editor_core/ae_pasteboard_constants.h>
#include <audio_editor_core/ae_pasteboard_utils.h>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::pasteboard_constants;

std::string pasting_file_module::data() const {
    std::map<std::string, std::string> map{{file_module_key::kind, file_module_kind::value},
                                           {file_module_key::file_frame, std::to_string(this->file_frame)},
                                           {file_module_key::length, std::to_string(this->length)}};
    return pasteboard_utils::to_data_string(map);
}

std::optional<pasting_file_module> pasting_file_module::make(std::string const &data) {
    auto const map = pasteboard_utils::to_data_map(data);

    if (map.contains(file_module_key::kind) && map.at(file_module_key::kind) == file_module_kind::value &&
        map.contains(file_module_key::file_frame) && map.contains(file_module_key::length)) {
        auto const file_frame_string = map.at(file_module_key::file_frame);
        auto const length_string = map.at(file_module_key::length);
        return pasting_file_module{.file_frame = std::stoll(file_frame_string), .length = std::stoul(length_string)};
    }

    return std::nullopt;
}

std::string yas::to_string(ae::pasting_file_module const &module) {
    return "{" + std::to_string(module.file_frame) + "," + std::to_string(module.length) + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::pasting_file_module const &value) {
    os << to_string(value);
    return os;
}
