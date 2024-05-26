//
//  marker_renaming_opener.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

#include <ae-core/project_editing/value_types/marker_index.hpp>

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

    marker_renaming_opener(marker_renaming_opener const &) = delete;
    marker_renaming_opener(marker_renaming_opener &&) = delete;
    marker_renaming_opener &operator=(marker_renaming_opener const &) = delete;
    marker_renaming_opener &operator=(marker_renaming_opener &&) = delete;
};
}  // namespace yas::ae
