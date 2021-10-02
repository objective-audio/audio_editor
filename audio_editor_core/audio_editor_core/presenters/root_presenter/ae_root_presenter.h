//
//  ae_root_presenter.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class project;

struct root_presenter {
    [[nodiscard]] static std::shared_ptr<root_presenter> make_shared(std::string const &project_id);

    [[nodiscard]] bool responds_to_action(action const);

    [[nodiscard]] observing::syncable observe_is_editing(std::function<void(bool const &)> &&);

   private:
    std::weak_ptr<project> const _project;
    observing::value::holder_ptr<bool> const _is_editing;
    observing::canceller_pool _pool;

    root_presenter(std::shared_ptr<project> const &);
};
}  // namespace yas::ae
