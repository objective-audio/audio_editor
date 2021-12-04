//
//  ae_file_module.cpp
//

#include "ae_file_module.h"

#include <audio_editor_core/ae_file_module_utils.h>

using namespace yas;
using namespace yas::ae;

bool file_module::operator==(file_module const &rhs) const {
    return this->identifier == rhs.identifier && this->range == rhs.range && this->file_frame == rhs.file_frame;
}

bool file_module::operator!=(file_module const &rhs) const {
    return !(*this == rhs);
}

bool file_module::is_equal_location(file_module const &rhs) const {
    return this->range == rhs.range && this->file_frame == rhs.file_frame;
}

std::optional<file_module> file_module::head_dropped(proc::frame_index_t const frame) const {
    if (file_module_utils::can_split_time_range(this->range, frame)) {
        return file_module{
            .range = {frame, static_cast<proc::length_t>(this->range.next_frame() - frame)},
            .file_frame = static_cast<proc::frame_index_t>(this->file_frame - this->range.frame + frame)};
    } else {
        return std::nullopt;
    }
}

std::optional<file_module> file_module::tail_dropped(proc::frame_index_t const frame) const {
    if (file_module_utils::can_split_time_range(this->range, frame)) {
        return file_module{.range = {this->range.frame, static_cast<proc::length_t>(frame - this->range.frame)},
                           .file_frame = this->file_frame};
    } else {
        return std::nullopt;
    }
}

file_module file_module::offset(proc::frame_index_t const offset) const {
    return file_module{.range = this->range.offset(offset), .file_frame = this->file_frame};
}

std::string yas::to_string(ae::file_module const &file_module) {
    return std::string{"{file_frame:" + std::to_string(file_module.file_frame) +
                       ", range:" + to_string(file_module.range) + "}"};
}
