//
//  project_setup_dialog_lifetime.cpp
//

#include "project_setup_dialog_lifetime.h"

#include <ae-core/project_format_dialog/features/project_format_setup.h>
#include <ae-core/project_format_dialog/features/project_setup.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_setup_dialog_lifetime> project_setup_dialog_lifetime::make_shared(
    project_setup_dialog_lifetime_id const &lifetime_id) {
    auto const format_setup = std::make_shared<ae::project_format_setup>();
    auto const setup = project_setup::make_shared(lifetime_id, format_setup.get());

    return std::make_shared<project_setup_dialog_lifetime>(lifetime_id, format_setup, setup);
}

project_setup_dialog_lifetime::project_setup_dialog_lifetime(project_setup_dialog_lifetime_id const &lifetime_id,
                                                             std::shared_ptr<project_format_setup> const &format_setup,
                                                             std::shared_ptr<project_setup> const &setup)
    : lifetime_id(lifetime_id), format_setup(format_setup), setup(setup) {
}
