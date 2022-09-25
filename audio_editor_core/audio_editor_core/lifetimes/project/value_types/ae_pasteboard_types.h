//
//  ae_pasteboard_types.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_json.h>

#include <string>
#include <variant>

namespace yas::ae {
enum class pasting_kind {
    file_module,
};

enum class pasteboard_event {
    fetched,
    file_module,
    cleared,
};

struct pasting_file_module final {
    std::string name;
    frame_index_t file_frame;
    time::range range;
    std::string file_name;

    [[nodiscard]] static std::optional<pasting_file_module> make(json_value const &);

    pasting_file_module(std::string const &name, frame_index_t const file_frame, time::range const &range,
                        std::string const &file_name);

    [[nodiscard]] json_value json() const;

    bool operator==(pasting_file_module const &) const;
    bool operator!=(pasting_file_module const &) const;
};

using pasting_value = std::variant<pasting_file_module>;
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::pasting_file_module const &);
}  // namespace yas

std::ostream &operator<<(std::ostream &os, yas::ae::pasting_file_module const &);
