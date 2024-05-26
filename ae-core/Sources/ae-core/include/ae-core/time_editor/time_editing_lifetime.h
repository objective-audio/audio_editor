//
//  time_editing_lifetime.h
//

#pragma once

#include <ae-core/global/value_types/project_sub_lifetime_id.h>
#include <cpp-utils/identifier.h>

#include <memory>
#include <optional>
#include <string>

namespace yas::ae {
class number_components;
class time_editor;
class time_editor_responder;
class time_editing_closer;
class time_editor_receiver;
class project_editing_lifetime;

struct time_editing_lifetime final {
    [[nodiscard]] static std::shared_ptr<time_editing_lifetime> make_shared(project_sub_lifetime_id const &,
                                                                            number_components const &);

    time_editing_lifetime(project_sub_lifetime_id const &, project_editing_lifetime const *, number_components const &);

    project_sub_lifetime_id const lifetime_id;
    std::shared_ptr<time_editor> const editor;
    std::shared_ptr<time_editing_closer> const closer;
    std::shared_ptr<time_editor_receiver> const receiver;

   private:
    time_editing_lifetime(time_editing_lifetime const &) = delete;
    time_editing_lifetime(time_editing_lifetime &&) = delete;
    time_editing_lifetime &operator=(time_editing_lifetime const &) = delete;
    time_editing_lifetime &operator=(time_editing_lifetime &&) = delete;
};
}  // namespace yas::ae
