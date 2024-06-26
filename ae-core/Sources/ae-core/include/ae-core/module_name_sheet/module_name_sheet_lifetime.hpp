//
//  module_name_sheet_lifetime.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <ae-core/global/value_types/project_sub_lifetime_id.h>

#include <ae-core/project_editing/value_types/module_index.hpp>
#include <memory>

namespace yas::ae {
class module_name_editor;

struct module_name_sheet_lifetime final {
    module_name_sheet_lifetime(project_sub_lifetime_id const &, module_index const &);

    project_sub_lifetime_id const lifetime_id;
    std::shared_ptr<module_name_editor> const editor;

   private:
    module_name_sheet_lifetime(module_name_sheet_lifetime const &) = delete;
    module_name_sheet_lifetime(module_name_sheet_lifetime &&) = delete;
    module_name_sheet_lifetime &operator=(module_name_sheet_lifetime const &) = delete;
    module_name_sheet_lifetime &operator=(module_name_sheet_lifetime &&) = delete;
};
}  // namespace yas::ae
