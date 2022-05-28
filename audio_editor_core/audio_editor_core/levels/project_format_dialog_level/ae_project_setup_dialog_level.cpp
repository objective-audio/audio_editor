//
//  ae_project_setup_dialog_level.cpp
//

#include "ae_project_setup_dialog_level.h"

#include <audio_editor_core/ae_project_format_setup.h>
#include <audio_editor_core/ae_project_setup.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_setup_dialog_level> project_setup_dialog_level::make_shared() {
    auto const format_setup = project_format_setup::make_shared();
    auto const setup = project_setup::make_shared(format_setup.get());

    return std::make_shared<project_setup_dialog_level>(format_setup, setup);
}

project_setup_dialog_level::project_setup_dialog_level(std::shared_ptr<project_format_setup> const &format_setup,
                                                       std::shared_ptr<project_setup> const &setup)
    : format_setup(format_setup), setup(setup) {
}
