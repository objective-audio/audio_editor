//
//  ae_root_presenter.h
//

#pragma once

#include <audio_editor_core/ae_root_presenter_dependency.h>

namespace yas::ae {
struct root_presenter final {
    [[nodiscard]] static std::shared_ptr<root_presenter> make_shared(std::string const &project_id);

    [[nodiscard]] observing::syncable observe_is_editing(std::function<void(bool const &)> &&);

   private:
    observing::value::holder_ptr<bool> const _is_editing;
    observing::canceller_pool _pool;

    root_presenter(std::shared_ptr<project_for_root_presenter> const &);

    root_presenter(root_presenter const &) = delete;
    root_presenter(root_presenter &&) = delete;
    root_presenter &operator=(root_presenter const &) = delete;
    root_presenter &operator=(root_presenter &&) = delete;
};
}  // namespace yas::ae
