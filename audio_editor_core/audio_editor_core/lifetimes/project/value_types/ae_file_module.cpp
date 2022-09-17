//
//  ae_file_module.cpp
//

#include "ae_file_module.h"

#include <audio_editor_core/ae_file_module_utils.h>

using namespace yas;
using namespace yas::ae;

file_module::params::params(std::string const &name, time::range const &range, frame_index_t const file_frame,
                            std::string const &file_name)
    : name(name), range(range), file_frame(file_frame), file_name(file_name) {
}

file_module::file_module(object_id const &identifier, params &&args)
    : file_module(identifier, args.name, args.range, args.file_frame, args.file_name) {
}

file_module::file_module(object_id const &identifier, std::string const &name, time::range const &range,
                         frame_index_t const file_frame, std::string const &file_name)
    : identifier(identifier), name(name), range(range), file_frame(file_frame), file_name(file_name) {
}

file_module::params file_module::parameters() const {
    return {this->name, this->range, this->file_frame, this->file_name};
}

std::optional<file_module::params> file_module::head_dropped(frame_index_t const frame) const {
    if (file_module_utils::can_split_time_range(this->range, frame)) {
        return file_module::params{this->name,
                                   {frame, static_cast<proc::length_t>(this->range.next_frame() - frame)},
                                   static_cast<frame_index_t>(this->file_frame - this->range.frame + frame),
                                   this->file_name};
    } else {
        return std::nullopt;
    }
}

std::optional<file_module::params> file_module::tail_dropped(frame_index_t const frame) const {
    if (file_module_utils::can_split_time_range(this->range, frame)) {
        return file_module::params{this->name,
                                   {this->range.frame, static_cast<proc::length_t>(frame - this->range.frame)},
                                   this->file_frame,
                                   this->file_name};
    } else {
        return std::nullopt;
    }
}

file_module::params file_module::offset(frame_index_t const offset) const {
    return file_module::params{this->name, this->range.offset(offset), this->file_frame, this->file_name};
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
