//
//  module_name_sheet_lifetime.cpp
//

#include "module_name_sheet_lifetime.hpp"

#include <ae-core/module_name_sheet/features/module_name_editor.hpp>

using namespace yas;
using namespace yas::ae;

module_name_sheet_lifetime::module_name_sheet_lifetime(project_sub_lifetime_id const &lifetime_id,
                                                       module_index const &index)
    : lifetime_id(lifetime_id), editor(module_name_editor::make_shared(lifetime_id, index)) {
}
