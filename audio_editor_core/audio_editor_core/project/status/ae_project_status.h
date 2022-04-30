//
//  ae_project_status.h
//

#pragma once

#include <audio_editor_core/ae_project_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct project_status final {
    [[nodiscard]] static std::shared_ptr<project_status> make_shared();

    project_state const &state() const;
    void set_state(project_state const &);

   private:
    observing::value::holder_ptr<project_state> const _state;

    project_status();
};
}  // namespace yas::ae
