//
//  ae_selected_file_module.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <audio_editor_core/ae_file_module_index.hpp>

namespace yas::ae {
struct selected_file_module final {
    time::range range;

    selected_file_module(time::range const &range);
};

struct selected_file_module_object final {
    object_id identifier;
    selected_file_module value;

    selected_file_module_object(object_id const &, selected_file_module const &);

    [[nodiscard]] file_module_index index() const;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::selected_file_module const &);
}  // namespace yas

std::ostream &operator<<(std::ostream &os, yas::ae::selected_file_module const &);
