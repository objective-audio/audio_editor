//
//  ae_project_setup_dialog_lifetime.cpp
//

#include "ae_project_setup_dialog_lifetime.h"

#include <audio_editor_core/ae_project_format_setup.h>
#include <audio_editor_core/ae_project_setup.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_setup_dialog_lifetime> project_setup_dialog_lifetime::make_shared() {
    auto const format_setup = project_format_setup::make_shared();
    auto const setup = project_setup::make_shared(format_setup.get());

    return std::make_shared<project_setup_dialog_lifetime>(format_setup, setup);
}

project_setup_dialog_lifetime::project_setup_dialog_lifetime(std::shared_ptr<project_format_setup> const &format_setup,
                                                             std::shared_ptr<project_setup> const &setup)
    : format_setup(format_setup), setup(setup) {
}
