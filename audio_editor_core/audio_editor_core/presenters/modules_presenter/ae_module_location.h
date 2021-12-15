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

    [[nodiscard]] static module_location make(ae::identifier const &, proc::time::range const &,
                                              uint32_t const sample_rate);
};
}  // namespace yas::ae
