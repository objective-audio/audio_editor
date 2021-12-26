//
//  ae_module_location.h
//

#pragma once

#include <audio_editor_core/ae_identifier.h>
#include <audio_editor_core/ae_module_location.h>
#include <processing/yas_processing_umbrella.h>

namespace yas::ae {
struct module_location {
    struct mesh_element {
        proc::time::range range;

        bool operator==(mesh_element const &rhs) const {
            return this->range == rhs.range;
        }

        bool operator!=(mesh_element const &rhs) const {
            return !(*this == rhs);
        }
    };

    identifier identifier;  // file_moduleからコピーする
    float x;
    float width;
    proc::time::range range;
    std::vector<std::optional<mesh_element>> mesh_elements;

    [[nodiscard]] static module_location make_value(ae::identifier const &, proc::time::range const &,
                                                    uint32_t const sample_rate);

    bool operator==(module_location const &rhs) const {
        return this->identifier == rhs.identifier && this->x == rhs.x && this->width == rhs.width &&
               this->range == rhs.range && equal(this->mesh_elements, rhs.mesh_elements);
    }

    bool operator!=(module_location const &rhs) const {
        return !(*this == rhs);
    }
};
}  // namespace yas::ae
