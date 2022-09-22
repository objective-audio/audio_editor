//
//  ae_file_ref.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <string>

namespace yas::ae {
struct file_ref final {
    std::string file_name;
};

struct file_ref_object final {
    object_id identifier;
    file_ref value;

    file_ref_object(object_id const &, file_ref &&);
};
}  // namespace yas::ae
