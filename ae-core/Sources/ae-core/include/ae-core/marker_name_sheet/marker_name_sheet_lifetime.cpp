//
//  marker_name_sheet_lifetime.cpp
//

#include "marker_name_sheet_lifetime.hpp"

#include <ae-core/marker_name_sheet/features/marker_name_editor.hpp>

using namespace yas;
using namespace yas::ae;

marker_name_sheet_lifetime::marker_name_sheet_lifetime(project_sub_lifetime_id const &lifetime_id,
                                                       marker_index const &index)
    : lifetime_id(lifetime_id), editor(marker_name_editor::make_shared(lifetime_id, index)) {
}
