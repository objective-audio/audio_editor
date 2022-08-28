//
//  ae_module_name_sheet_lifetime.cpp
//

#include "ae_module_name_sheet_lifetime.hpp"

#include <audio_editor_core/ae_module_name_editor.hpp>

using namespace yas;
using namespace yas::ae;

module_name_sheet_lifetime::module_name_sheet_lifetime(project_sub_lifetime_id const &lifetime_id,
                                                       time::range const &range)
    : lifetime_id(lifetime_id), editor(module_name_editor::make_shared(lifetime_id, range)) {
}
