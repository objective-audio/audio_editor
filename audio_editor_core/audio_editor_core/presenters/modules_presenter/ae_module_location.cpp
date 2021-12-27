//
//  ae_module_location.cpp
//

#include "ae_module_location.h"

#include <audio_editor_core/ae_file_module.h>

using namespace yas;
using namespace yas::ae;

module_location module_location::make_value(ae::identifier const &identifier, proc::time::range const &range,
                                            proc::frame_index_t const file_frame, uint32_t const sample_rate,
                                            std::vector<std::optional<mesh_element>> const &mesh_elements) {
    float const x = static_cast<double>(range.frame) / static_cast<double>(sample_rate);
    float const width = static_cast<double>(range.length) / static_cast<double>(sample_rate);
    return module_location{.identifier = identifier,
                           .x = x,
                           .width = width,
                           .range = range,
                           .file_frame = file_frame,
                           .mesh_elements = mesh_elements};
}

module_location module_location::make_value(file_module const &file_module, uint32_t const sample_rate,
                                            proc::time::range const &space_range, float const width_per_sec) {
    return make_value(file_module.identifier, file_module.range, file_module.file_frame, sample_rate, {});
}
