//
//  ae_module_location.cpp
//

#include "ae_module_location.h"

#include <audio_editor_core/ae_file_module.h>
#include <cpp_utils/yas_fast_each.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

bool module_location::mesh_element::operator==(mesh_element const &rhs) const {
    return this->rect_count == rhs.rect_count && this->range == rhs.range;
}

bool module_location::mesh_element::operator!=(mesh_element const &rhs) const {
    return !(*this == rhs);
}

module_location module_location::make_value(yas::identifier const &identifier, std::string const &name,
                                            time::range const &range, frame_index_t const file_frame,
                                            uint32_t const sample_rate,
                                            std::vector<std::optional<mesh_element>> const &mesh_elements,
                                            float const scale) {
    return module_location{.identifier = identifier,
                           .name = name,
                           .sample_rate = sample_rate,
                           .range = range,
                           .file_frame = file_frame,
                           .mesh_elements = mesh_elements,
                           .scale = scale};
}

module_location module_location::make_value(file_module const &file_module, uint32_t const sample_rate,
                                            time::range const &space_range, float const scale) {
    std::vector<std::optional<mesh_element>> mesh_elements;

    uint32_t const mesh_width_interval = module_location::mesh_element::max_length;
    double const width = static_cast<double>(file_module.range.length) / static_cast<double>(sample_rate);
    double const total_mesh_width = width * scale;
    uint32_t const floored_mesh_width = static_cast<uint32_t>(std::floor(total_mesh_width));
    uint32_t const ceiled_mesh_width = static_cast<uint32_t>(std::ceil(total_mesh_width));
    bool const has_fraction = floored_mesh_width != ceiled_mesh_width;
    uint32_t const total_rect_count = floored_mesh_width + (has_fraction ? 1 : 0);

    uint32_t current_frame = 0;
    uint32_t const total_length = static_cast<uint32_t>(file_module.range.length);
    uint32_t const mod_rect_count = total_rect_count % mesh_width_interval;
    auto const data_count = total_rect_count / mesh_width_interval + (mod_rect_count ? 1 : 0);
    auto each = make_fast_each(data_count);
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        uint32_t const rect_count =
            ((idx == (data_count - 1)) && mod_rect_count) ? mod_rect_count : mesh_width_interval;
        double const next_position = static_cast<double>((idx + 1) * mesh_width_interval) / total_mesh_width *
                                     static_cast<double>(file_module.range.length);
        auto const next_frame = std::min(static_cast<uint32_t>(next_position), total_length);
        auto const range = time::range{current_frame, next_frame - current_frame};

        auto const offset_range = range.offset(file_module.range.frame);
        if (offset_range.intersected(space_range)) {
            mesh_elements.emplace_back(mesh_element{.rect_count = rect_count, .range = range});
        } else {
            mesh_elements.emplace_back(std::nullopt);
        }

        current_frame = next_frame;
    }

    return make_value(file_module.identifier, file_module.name, file_module.range, file_module.file_frame, sample_rate,
                      mesh_elements, scale);
}

float module_location::x() const {
    return static_cast<double>(this->range.frame) / static_cast<double>(this->sample_rate);
}

float module_location::width() const {
    return static_cast<double>(this->range.length) / static_cast<double>(this->sample_rate);
}

uint32_t module_location::total_rect_count() const {
    uint32_t total_rect_count = 0;

    for (auto const &element : this->mesh_elements) {
        total_rect_count += element.value().rect_count;
    }

    return total_rect_count;
}

std::optional<float> module_location::element_offset_x(std::size_t const idx) const {
    if (auto const &element = this->mesh_elements.at(idx)) {
        return static_cast<double>(element.value().range.frame) / static_cast<double>(this->sample_rate);
    } else {
        return std::nullopt;
    }
}

bool module_location::operator==(module_location const &rhs) const {
    return this->identifier == rhs.identifier && this->name == rhs.name && this->sample_rate == rhs.sample_rate &&
           this->range == rhs.range && this->file_frame == rhs.file_frame && this->scale == rhs.scale &&
           equal(this->mesh_elements, rhs.mesh_elements);
}

bool module_location::operator!=(module_location const &rhs) const {
    return !(*this == rhs);
}
