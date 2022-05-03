//
//  ae_time_editor_level.cpp
//

#include "ae_time_editor_level.h"

#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_responder.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_level> time_editor_level::make_shared(std::string const &project_id,
                                                                  number_components const &components,
                                                                  std::optional<std::size_t> const unit_idx) {
    return std::shared_ptr<time_editor_level>(new time_editor_level{project_id, components, unit_idx});
}

time_editor_level::time_editor_level(std::string const &project_id, number_components const &components,
                                     std::optional<std::size_t> const unit_idx)
    : project_id(project_id),
      editor(time_editor::make_shared(components, unit_idx)),
      responder(time_editor_responder::make_shared(this->editor)) {
}
