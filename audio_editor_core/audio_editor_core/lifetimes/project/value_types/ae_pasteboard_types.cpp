//
//  ae_pasteboard_types.cpp
//

#include "ae_pasteboard_types.h"

#include <audio_editor_core/ae_pasteboard_constants.h>
#include <cpp_utils/yas_json.h>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::pasteboard_constants;

std::string pasting_file_module::data() const {
    assert(!this->file_name.empty());
    json_map map{{file_module_key::kind, json_value{file_module_kind::value}},
                 {file_module_key::name, json_value{this->name}},
                 {file_module_key::file_frame, json_value{this->file_frame}},
                 {file_module_key::frame, json_value{this->range.frame}},
                 {file_module_key::length, json_value{int64_t(this->range.length)}},
                 {file_module_key::file_name, json_value{this->file_name}}};
    return to_json_string(json_value{std::move(map)});
}

std::optional<pasting_file_module> pasting_file_module::make_value(std::string const &data) {
    auto const json_value = to_json_value(data);

    if (!json_value.map.has_value()) {
        return std::nullopt;
    }

    auto const &map_value = json_value.map.value();

    if (map_value.contains(file_module_key::kind) && map_value.at(file_module_key::kind).string.has_value() &&
        map_value.at(file_module_key::kind).string.value() == file_module_kind::value &&
        map_value.contains(file_module_key::name) && map_value.at(file_module_key::name).string.has_value() &&
        map_value.contains(file_module_key::file_frame) &&
        map_value.at(file_module_key::file_frame).integer.has_value() && map_value.contains(file_module_key::length) &&
        map_value.at(file_module_key::length).integer.has_value() && map_value.contains(file_module_key::file_name) &&
        map_value.at(file_module_key::file_name).string.has_value()) {
        auto const name = map_value.at(file_module_key::name).string.value();
        auto const file_frame = map_value.at(file_module_key::file_frame).integer.value();
        auto const frame = map_value.at(file_module_key::frame).integer.value();
        auto const length = map_value.at(file_module_key::length).integer.value();
        auto const file_name = map_value.at(file_module_key::file_name).string.value();
        return pasting_file_module{
            .name = name, .file_frame = file_frame, .range = {frame, length_t(length)}, .file_name = file_name};
    }

    return std::nullopt;
}

std::string yas::to_string(ae::pasting_file_module const &module) {
    return "{name:" + module.name + ", file_frame:" + std::to_string(module.file_frame) +
           ", range:" + to_string(module.range) + ", file_name:" + module.file_name + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::pasting_file_module const &value) {
    os << to_string(value);
    return os;
}
