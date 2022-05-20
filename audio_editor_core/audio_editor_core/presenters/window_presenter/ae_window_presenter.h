//
//  ae_window_presenter.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>

namespace yas::ae {
struct window_presenter final {
    [[nodiscard]] static std::shared_ptr<window_presenter> make_shared(project_id const &project_id);
    [[nodiscard]] static std::shared_ptr<window_presenter> make_shared(
        project_id const &project_id, url const &file_url,
        std::shared_ptr<project_closer_for_window_presenter> const &);

    project_id const project_id;

    std::string title() const;

    bool should_close();

   private:
    url const _file_url;
    std::weak_ptr<project_closer_for_window_presenter> _closer;

    window_presenter(ae::project_id const &project_id, url const &file_url,
                     std::shared_ptr<project_closer_for_window_presenter> const &);

    window_presenter(window_presenter const &) = delete;
    window_presenter(window_presenter &&) = delete;
    window_presenter &operator=(window_presenter const &) = delete;
    window_presenter &operator=(window_presenter &&) = delete;
};
}  // namespace yas::ae
