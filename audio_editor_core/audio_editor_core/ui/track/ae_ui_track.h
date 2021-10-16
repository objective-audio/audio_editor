//
//  ae_ui_track.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class track_presenter;
class ui_modules;
class ui_markers;

struct ui_track final {
    std::shared_ptr<ui::node> const node() const;

    [[nodiscard]] static std::shared_ptr<ui_track> make_shared(std::shared_ptr<ui::standard> const &,
                                                               std::string const &project_id);

   private:
    std::shared_ptr<track_presenter> const _presenter;
    std::shared_ptr<ui::node> const _root_node;
    std::shared_ptr<ui::node> const _time_node;
    std::shared_ptr<ui_modules> const _modules;
    std::shared_ptr<ui_markers> const _markers;

    observing::canceller_pool _pool;

    ui_track(std::shared_ptr<ui::standard> const &, std::shared_ptr<track_presenter> const &,
             std::shared_ptr<ui_modules> const &, std::shared_ptr<ui_markers> const &);
};
}  // namespace yas::ae
