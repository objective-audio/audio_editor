//
//  ae_pasteboard_types.h
//

#pragma once

#include <processing/yas_processing_time.h>

namespace yas::ae {
enum class pasteboard_event {
    fetched,
    file_module,
    reverted,
};

struct pasting_file_module {
    proc::frame_index_t file_frame;
    proc::length_t length;

    std::string data() const;

    static std::optional<pasting_file_module> make(std::string const &);
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::pasting_file_module const &);
}

std::ostream &operator<<(std::ostream &os, yas::ae::pasting_file_module const &);
