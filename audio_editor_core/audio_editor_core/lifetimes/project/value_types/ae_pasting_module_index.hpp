//
//  ae_pasting_module_index.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
struct pasting_module_index final {
    identifier identifier;
    time::range range;

    pasting_module_index(yas::identifier const &, time::range const &);

    bool operator==(pasting_module_index const &) const;
    bool operator!=(pasting_module_index const &) const;
    bool operator<(pasting_module_index const &) const;
};
}  // namespace yas::ae
