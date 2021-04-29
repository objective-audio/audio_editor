//
//  ae_project_view_presenter_utils.cpp
//

#include "ae_project_view_presenter_utils.h"

using namespace yas;
using namespace yas::ae;

std::string project_view_presenter_utils::to_label_string(project_state const &state) {
    switch (state) {
        case ae::project_state::launching:
            return "launching";
        case ae::project_state::loading:
            return "loading";
        case ae::project_state::editing:
            return "editing";
        case ae::project_state::failure:
            return "failure";
        case ae::project_state::closing:
            return "closing";
    }
}
