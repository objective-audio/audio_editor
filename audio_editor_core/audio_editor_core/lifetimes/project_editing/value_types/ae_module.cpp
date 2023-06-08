//
//  ae_module.cpp
//

#include "ae_module.h"

#include <audio_editor_core/ae_module_utils.h>

using namespace yas;
using namespace yas::ae;

module::module(std::string const &name, time::range const &range, track_index_t const track,
               frame_index_t const file_frame, std::string const &file_name)
    : name(name), range(range), track(track), file_frame(file_frame), file_name(file_name) {
}

module_object::module_object(object_id const &identifier, module &&value)
    : identifier(identifier), value(std::move(value)) {
}

std::optional<module> module::head_dropped(frame_index_t const frame) const {
    if (module_utils::can_split_time_range(this->range, frame)) {
        return module{this->name,
                      {frame, static_cast<proc::length_t>(this->range.next_frame() - frame)},
                      this->track,
                      static_cast<frame_index_t>(this->file_frame - this->range.frame + frame),
                      this->file_name};
    } else {
        return std::nullopt;
    }
}

std::optional<module> module::tail_dropped(frame_index_t const frame) const {
    if (module_utils::can_split_time_range(this->range, frame)) {
        return module{this->name,
                      {this->range.frame, static_cast<proc::length_t>(frame - this->range.frame)},
                      this->track,
                      this->file_frame,
                      this->file_name};
    } else {
        return std::nullopt;
    }
}

module module::offset(frame_index_t const offset) const {
    return module{this->name, this->range.offset(offset), this->track, this->file_frame, this->file_name};
}

module_index module_object::index() const {
    return {this->identifier, this->value.range};
}

std::string yas::to_string(ae::module const &module) {
    return std::string{"{file_frame:" + std::to_string(module.file_frame) + ", range:" + to_string(module.range) +
                       ", track:" + std::to_string(module.track) + ", name:" + module.name +
                       ", file_name:" + module.file_name + "}"};
}

std::ostream &operator<<(std::ostream &os, yas::ae::module const &value) {
    os << to_string(value);
    return os;
}
