//
//  ae_module_location.h
//

#pragma once

#include <audio_editor_core/ae_identifier.h>

namespace yas::ae {
struct module_location {
    identifier identifier;  // file_moduleからコピーする
    float x;
    float width;
};
}  // namespace yas::ae
