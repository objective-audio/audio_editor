//
//  ae_scroller_presenter.h
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

#include <memory>

namespace yas::ae {
class player;
class zooming_pair;

struct scroller_presenter final {
    [[nodiscard]] static std::shared_ptr<scroller_presenter> make_shared(window_lifetime_id const &);

    scroller_presenter(project_format const &, std::shared_ptr<player> const &, std::shared_ptr<zooming_pair> const &);

    [[nodiscard]] float current_position() const;
    [[nodiscard]] double horizontal_zooming_scale() const;

   private:
    project_format const _project_format;
    std::weak_ptr<player> _player;
    std::weak_ptr<zooming_pair> _zooming_pair;
};
}  // namespace yas::ae
