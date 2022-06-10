//
//  ae_file_module.cpp
//

#include "ae_file_module.h"

#include <audio_editor_core/ae_file_module_utils.h>

using namespace yas;
using namespace yas::ae;

bool file_module::is_equal_location(file_module const &rhs) const {
    return this->range == rhs.range && this->file_frame == rhs.file_frame;
}

std::optional<file_module> file_module::head_dropped(frame_index_t const frame) const {
    if (file_module_utils::can_split_time_range(this->range, frame)) {
        return file_module{.name = this->name,
                           .range = {frame, static_cast<proc::length_t>(this->range.next_frame() - frame)},
                           .file_frame = static_cast<frame_index_t>(this->file_frame - this->range.frame + frame),
                           .file_name = this->file_name};
    } else {
        return std::nullopt;
    }
}

std::optional<file_module> file_module::tail_dropped(frame_index_t const frame) const {
    if (file_module_utils::can_split_time_range(this->range, frame)) {
        return file_module{.name = this->name,
                           .range = {this->range.frame, static_cast<proc::length_t>(frame - this->range.frame)},
                           .file_frame = this->file_frame,
                           .file_name = this->file_name};
    } else {
        return std::nullopt;
    }
}

file_module file_module::offset(frame_index_t const offset) const {
    return file_module{.name = this->name,
                       .range = this->range.offset(offset),
                       .file_frame = this->file_frame,
                       .file_name = this->file_name};
}

std::string yas::to_string(ae::file_module const &file_module) {
    return std::string{"{file_frame:" + std::to_string(file_module.file_frame) +
                       ", range:" + to_string(file_module.range) + ", name:" + file_module.name +
                       ", file_name:" + file_module.file_name + "}"};
}

std::ostream &operator<<(std::ostream &os, yas::ae::file_module const &value) {
    os << to_string(value);
    return os;
}
