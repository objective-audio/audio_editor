//
//  ae_file_ref.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <string>

namespace yas::ae {
struct file_ref final {
    struct params {
        std::string file_name;
    };

    object_id identifier;
    std::string file_name;

    file_ref(object_id const &, params &&);
    file_ref(object_id const &, std::string const &file_name);

    [[nodiscard]] params parameters() const;
};
}  // namespace yas::ae
