//
//  ae_scroller_presenter.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>

#include <memory>

namespace yas::ae {
class player;
class zooming;

struct scroller_presenter final {
    [[nodiscard]] static std::shared_ptr<scroller_presenter> make_shared(std::string const &project_id);

    [[nodiscard]] float current_position() const;
    [[nodiscard]] double horizontal_zooming_scale() const;

   private:
    file_info const _file_info;
    std::weak_ptr<player> _player;
    std::weak_ptr<zooming> _horizontal_zooming;
    std::weak_ptr<zooming> _vertical_zooming;

    scroller_presenter(file_info const &, std::shared_ptr<player> const &,
                       std::shared_ptr<zooming> const &horizontal_zooming,
                       std::shared_ptr<zooming> const &vertical_zooming);
};
}  // namespace yas::ae
