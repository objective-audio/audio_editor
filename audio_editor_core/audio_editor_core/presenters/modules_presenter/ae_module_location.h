//
//  ae_module_location.h
//

#pragma once

#include <audio_editor_core/ae_identifier.h>
#include <audio_editor_core/ae_module_location.h>
#include <processing/yas_processing_umbrella.h>

namespace yas::ae {
struct module_location {
    identifier identifier;  // file_moduleからコピーする
    float x;
    float width;
    proc::time::range range;

    [[nodiscard]] static module_location make_value(ae::identifier const &, proc::time::range const &,
                                                    uint32_t const sample_rate);

    bool operator==(module_location const &rhs) const {
        return this->identifier == rhs.identifier && this->x == rhs.x && this->width == rhs.width &&
               this->range == rhs.range;
    }

    bool operator!=(module_location const &rhs) const {
        return !(*this == rhs);
    }
};
}  // namespace yas::ae
