//
//  ae_time_editor_status.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct time_editor_status final {
    [[nodiscard]] static std::shared_ptr<time_editor_status> make_shared();

    void finish();
    void cancel();

    bool is_finished() const;
    bool is_ended() const;
    bool can_finish() const;
    bool can_cancel() const;

   private:
    enum class state {
        editing,
        finished,
        canceled,
    };

    state _state;

    time_editor_status();
};
}  // namespace yas::ae
