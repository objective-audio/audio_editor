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
    std::string file_name;

    file_ref_object(object_id const &, file_ref &&);
    file_ref_object(object_id const &, std::string const &file_name);
};
}  // namespace yas::ae