//
//  ae_time_editor_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_project_sub_lifetime_id.h>
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
class project_editing_lifetime;

struct time_editor_lifetime final {
    [[nodiscard]] static std::shared_ptr<time_editor_lifetime> make_shared(project_sub_lifetime_id const &,
                                                                           number_components const &);

    time_editor_lifetime(project_sub_lifetime_id const &, project_editing_lifetime const *, number_components const &);

    project_sub_lifetime_id const lifetime_id;
    std::shared_ptr<time_editor> const editor;
    std::shared_ptr<time_editor_closer> const closer;
    std::shared_ptr<time_editor_receiver> const receiver;

   private:
    time_editor_lifetime(time_editor_lifetime const &) = delete;
    time_editor_lifetime(time_editor_lifetime &&) = delete;
    time_editor_lifetime &operator=(time_editor_lifetime const &) = delete;
    time_editor_lifetime &operator=(time_editor_lifetime &&) = delete;
};
}  // namespace yas::ae
