//
//  ae_window_presenter.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>

namespace yas::ae {
class project_url;

struct window_presenter final {
    [[nodiscard]] static std::shared_ptr<window_presenter> make_shared(project_id const &project_id);

    window_presenter(ae::project_id const &project_id, std::shared_ptr<project_url> const &,
                     std::shared_ptr<project_closer_for_window_presenter> const &);

    project_id const project_id;

    std::string title() const;

    bool should_close();

   private:
    std::shared_ptr<project_url> const _project_url;
    std::weak_ptr<project_closer_for_window_presenter> _closer;

    window_presenter(window_presenter const &) = delete;
    window_presenter(window_presenter &&) = delete;
    window_presenter &operator=(window_presenter const &) = delete;
    window_presenter &operator=(window_presenter &&) = delete;
};
}  // namespace yas::ae
