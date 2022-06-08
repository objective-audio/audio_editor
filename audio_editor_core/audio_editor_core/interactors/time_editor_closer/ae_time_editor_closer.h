//
//  ae_time_editor_closer.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_identifier.h>

#include <memory>
#include <optional>

namespace yas::ae {
class time_editor;
class time_editor_state;
class project_modal_lifecycle;
class responder_stack;
class timing;
class player;

struct time_editor_closer final {
    [[nodiscard]] static std::shared_ptr<time_editor_closer> make_shared(project_id const &project_id,
                                                                         identifier const lifetime_instance_id,
                                                                         time_editor *);

    time_editor_closer(identifier const lifetime_instance_id, time_editor *, project_modal_lifecycle *,
                       responder_stack *, timing *, player *);

    void finish();
    void cancel();

    bool can_finish();
    bool can_cancel();

   private:
    identifier const _lifetime_instance_id;

    struct dependencies {
        time_editor *const editor;
        project_modal_lifecycle *const modal_lifecycle;
        responder_stack *const responder_stack;
        timing *const timing;
        player *const player;
    };

    std::optional<dependencies> _dependencies;

    void _finalize();
};
}  // namespace yas::ae
