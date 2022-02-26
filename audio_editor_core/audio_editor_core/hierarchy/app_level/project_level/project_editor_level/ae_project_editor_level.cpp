//
//  ae_project_editor_level.cpp
//

#include "ae_project_editor_level.h"

#include <audio_editor_core/ae_nudging.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_level> project_editor_level::make_shared(std::string const &identifier,
                                                                        file_info const &file_info) {
    return std::shared_ptr<project_editor_level>(new project_editor_level{identifier, file_info});
}

project_editor_level::project_editor_level(std::string const &identifier, file_info const &file_info)
    : timing(timing::make_shared(file_info.sample_rate)),
      nudging(nudging::make_shared(this->timing)),
      editor(project_editor::make_shared(identifier, file_info, this->nudging, this->timing)) {
}
