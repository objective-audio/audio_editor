//
//  ae_file_module_index.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

namespace yas::ae {
struct file_module_index final {
    object_id object_id;
    time::range range;

    file_module_index(ae::object_id const &, time::range const &);

    bool operator==(file_module_index const &) const;
    bool operator!=(file_module_index const &) const;
    bool operator<(file_module_index const &) const;
};
}  // namespace yas::ae
