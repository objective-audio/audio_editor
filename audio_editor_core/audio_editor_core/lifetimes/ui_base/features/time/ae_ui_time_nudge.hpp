//
//  ae_ui_time_nudge.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class color;
class ui_time_numbers;
class time_numbers_presenter;
class time_nudge_presenter;

struct ui_time_nudge final {
    [[nodiscard]] static std::shared_ptr<ui_time_nudge> make_shared(project_lifetime_id const &, ui_time_numbers *,
                                                                    std::shared_ptr<ui::node> const &);
    ui_time_nudge(ae::color *, std::shared_ptr<ui::view_look> const &, ui_time_numbers *,
                  std::shared_ptr<time_numbers_presenter> const &, std::shared_ptr<time_nudge_presenter> const &,
                  std::shared_ptr<ui::node> const &);

   private:
    ae::color *const _color;
    ui_time_numbers *const _time_numbers;
    std::shared_ptr<ui::rect_plane> const _nudge_plane;
    std::shared_ptr<time_numbers_presenter> const _unit_presenter;
    std::shared_ptr<time_nudge_presenter> const _nudge_presenter;

    observing::canceller_pool _pool;

    ui_time_nudge(ui_time_nudge const &) = delete;
    ui_time_nudge(ui_time_nudge &&) = delete;
    ui_time_nudge &operator=(ui_time_nudge const &) = delete;
    ui_time_nudge &operator=(ui_time_nudge &&) = delete;

    void _update_nudge_position();
};
}  // namespace yas::ae
