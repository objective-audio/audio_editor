//
//  ae_marker_renaming_opener.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <audio_editor_core/ae_marker_index.hpp>
#include <memory>

namespace yas::ae {
class project_modal_lifecycle;
class editing_status;
class deselector;

struct marker_renaming_opener final {
    marker_renaming_opener(project_modal_lifecycle *, editing_status const *, deselector *);

    [[nodiscard]] bool can_begin_renaming() const;
    void begin_renaming(marker_index const &);

   private:
    project_modal_lifecycle *const _lifecycle;
    editing_status const *const _editing_status;
    deselector *const _deselector;
};
}  // namespace yas::ae
