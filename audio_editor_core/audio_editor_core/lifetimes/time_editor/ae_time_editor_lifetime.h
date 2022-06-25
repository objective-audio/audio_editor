//
//  ae_time_editor_lifetime.h
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
class time_editor_receiver;

struct time_editor_lifetime final {
    [[nodiscard]] static std::shared_ptr<time_editor_lifetime> make_shared(project_id const &project_id,
                                                                           number_components const &,
                                                                           std::optional<std::size_t> const unit_idx);

    time_editor_lifetime(ae::project_id const &project_id, number_components const &,
                         std::optional<std::size_t> const unit_idx);

    identifier const instance_id;
    project_id const project_id;
    std::shared_ptr<time_editor> const editor;
    std::shared_ptr<time_editor_closer> const closer;
    std::shared_ptr<time_editor_responder> const responder;
    std::shared_ptr<time_editor_receiver> const receiver;
};
}  // namespace yas::ae
