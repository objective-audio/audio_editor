//
//  ae_window_presenter.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>

namespace yas::ae {
struct window_presenter {
    static std::shared_ptr<window_presenter> make_shared(uintptr_t const project_id);
    static std::shared_ptr<window_presenter> make_shared(project_ptr const &);

    std::string title() const;

    bool should_close();

   private:
    project_ptr _project;

    window_presenter(project_ptr const &);

    window_presenter(window_presenter const &) = delete;
    window_presenter(window_presenter &&) = delete;
    window_presenter &operator=(window_presenter const &) = delete;
    window_presenter &operator=(window_presenter &&) = delete;
};
}  // namespace yas::ae
