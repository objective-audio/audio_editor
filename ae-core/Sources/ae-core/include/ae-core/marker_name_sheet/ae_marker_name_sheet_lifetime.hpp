//
//  ae_marker_name_sheet_lifetime.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_common_types.h>
#include <ae-core/global/value_types/ae_project_sub_lifetime_id.h>

#include <ae-core/project_editing/value_types/ae_marker_index.hpp>
#include <memory>

namespace yas::ae {
class marker_name_editor;

struct marker_name_sheet_lifetime final {
    marker_name_sheet_lifetime(project_sub_lifetime_id const &, marker_index const &);

    project_sub_lifetime_id const lifetime_id;
    std::shared_ptr<marker_name_editor> const editor;

   private:
    marker_name_sheet_lifetime(marker_name_sheet_lifetime const &) = delete;
    marker_name_sheet_lifetime(marker_name_sheet_lifetime &&) = delete;
    marker_name_sheet_lifetime &operator=(marker_name_sheet_lifetime const &) = delete;
    marker_name_sheet_lifetime &operator=(marker_name_sheet_lifetime &&) = delete;
};
}  // namespace yas::ae
