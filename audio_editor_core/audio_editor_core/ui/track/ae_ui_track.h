//
//  ae_ui_track.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class track_presenter;
class ui_module_plane;

struct ui_track final {
    std::shared_ptr<ui::node> const node() const;

    [[nodiscard]] static std::shared_ptr<ui_track> make_shared(std::shared_ptr<ui::standard> const &,
                                                               std::string const &project_id);

   private:
    std::shared_ptr<track_presenter> const _presenter;
    std::shared_ptr<ui::node> const _node;
    std::shared_ptr<ui_module_plane> const _module_plane;

    observing::canceller_pool _pool;

    ui_track(std::shared_ptr<ui::standard> const &, std::shared_ptr<track_presenter> const &);
};
}  // namespace yas::ae
