//
//  ae_time_editor_level.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_identifier.h>

#include <memory>
#include <optional>
#include <string>

namespace yas::ae {
class number_components;
class time_editor;
class time_editor_responder;
class time_editor_closer;
class responder_stack;

struct time_editor_level final {
    [[nodiscard]] static std::shared_ptr<time_editor_level> make_shared(project_id const &project_id,
                                                                        number_components const &,
                                                                        std::optional<std::size_t> const unit_idx);

    identifier const instance_id;
    project_id const project_id;
    std::shared_ptr<time_editor> const editor;
    std::shared_ptr<time_editor_closer> const closer;
    std::shared_ptr<time_editor_responder> const responder;

   private:
    time_editor_level(ae::project_id const &project_id, number_components const &,
                      std::optional<std::size_t> const unit_idx);
};
}  // namespace yas::ae
