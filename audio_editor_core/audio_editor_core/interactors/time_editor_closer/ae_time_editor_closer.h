//
//  ae_time_editor_closer.h
//

#pragma once

#include <audio_editor_core/ae_window_sub_lifetime_id.h>
#include <cpp_utils/yas_identifier.h>

#include <memory>
#include <optional>

namespace yas::ae {
class time_editor;
class time_editor_state;
class project_modal_lifecycle;
class timing;
class player;

struct time_editor_closer final {
    [[nodiscard]] static std::shared_ptr<time_editor_closer> make_shared(time_editor_lifetime_id const &,
                                                                         identifier const lifetime_instance_id,
                                                                         time_editor *);

    time_editor_closer(time_editor_lifetime_id const &, time_editor *, project_modal_lifecycle *, timing *, player *);

    void finish();
    void cancel();

    bool can_finish();
    bool can_cancel();

   private:
    time_editor_lifetime_id const _lifetime_id;

    struct dependencies {
        time_editor *const editor;
        project_modal_lifecycle *const modal_lifecycle;
        timing *const timing;
        player *const player;
    };

    std::optional<dependencies> _dependencies;

    void _finalize();
};
}  // namespace yas::ae
