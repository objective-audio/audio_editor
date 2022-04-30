//
//  ae_window_presenter.h
//

#pragma once

#include <audio_editor_core/ae_window_presenter_dependency.h>

namespace yas::ae {
struct window_presenter final {
    static std::shared_ptr<window_presenter> make_shared(std::string const &project_id);
    static std::shared_ptr<window_presenter> make_shared(std::string const &project_id, url const &file_url,
                                                         std::shared_ptr<project_closer_for_window_presenter> const &);

    std::string const project_id;

    std::string title() const;

    bool should_close();

   private:
    url const _file_url;
    std::shared_ptr<project_closer_for_window_presenter> _closer;

    window_presenter(std::string const &project_id, url const &file_url,
                     std::shared_ptr<project_closer_for_window_presenter> const &);

    window_presenter(window_presenter const &) = delete;
    window_presenter(window_presenter &&) = delete;
    window_presenter &operator=(window_presenter const &) = delete;
    window_presenter &operator=(window_presenter &&) = delete;
};
}  // namespace yas::ae
