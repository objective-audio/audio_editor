//
//  ae_window_presenter.h
//

#pragma once

#include <cstdint>
#include <memory>

namespace yas::ae {
class project;

struct window_presenter {
    static std::shared_ptr<window_presenter> make_shared(std::string const &project_id);
    static std::shared_ptr<window_presenter> make_shared(std::shared_ptr<project> const &);

    std::string title() const;

    bool should_close();

   private:
    std::shared_ptr<project> _project;

    window_presenter(std::shared_ptr<project> const &);

    window_presenter(window_presenter const &) = delete;
    window_presenter(window_presenter &&) = delete;
    window_presenter &operator=(window_presenter const &) = delete;
    window_presenter &operator=(window_presenter &&) = delete;
};
}  // namespace yas::ae
