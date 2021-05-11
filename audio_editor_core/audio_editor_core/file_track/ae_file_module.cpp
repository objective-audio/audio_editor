//
//  ae_file_module.cpp
//

#include "ae_file_module.h"

using namespace yas;
using namespace yas::ae;

bool file_module::operator==(file_module const &rhs) const {
    return this->range == rhs.range && this->file_frame == rhs.file_frame;
}

bool file_module::operator!=(file_module const &rhs) const {
    return !(*this == rhs);
}

bool file_module::can_split(proc::frame_index_t const frame) const {
    return this->range.is_contain(frame) && this->range.frame != frame;
}

std::optional<file_module> file_module::head_dropped(proc::frame_index_t const frame) const {
    if (this->can_split(frame)) {
        return file_module{
            .range = {frame, static_cast<proc::length_t>(this->range.next_frame() - frame)},
            .file_frame = static_cast<proc::frame_index_t>(this->file_frame - this->range.frame + frame)};
    } else {
        return std::nullopt;
    }
}

std::optional<file_module> file_module::tail_dropped(proc::frame_index_t const frame) const {
    if (this->can_split(frame)) {
        return file_module{.range = {this->range.frame, static_cast<proc::length_t>(frame - this->range.frame)},
                           .file_frame = this->file_frame};
    } else {
        return std::nullopt;
    }
}
