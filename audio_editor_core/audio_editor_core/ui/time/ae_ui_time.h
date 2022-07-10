//
//  ae_ui_time.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class time_presenter;
class project_action_sender;
class color;

struct ui_time final {
    [[nodiscard]] static std::shared_ptr<ui_time> make_shared(window_lifetime_id const &,
                                                              std::shared_ptr<ui::standard> const &,
                                                              std::shared_ptr<ui::texture> const &);

    ui_time(std::shared_ptr<ui::standard> const &, std::shared_ptr<ui::texture> const &,
            std::shared_ptr<ae::color> const &, std::shared_ptr<time_presenter> const &,
            std::shared_ptr<project_action_sender> const &);

    std::shared_ptr<ui::node> const node;

   private:
    struct button_element {
        std::shared_ptr<ui::button> button;
        observing::cancellable_ptr canceller;
    };

    std::shared_ptr<time_presenter> const _presenter;
    std::weak_ptr<project_action_sender> const _action_controller;
    std::weak_ptr<ui::standard> const _standard;
    std::shared_ptr<ae::color> const _color;
    std::shared_ptr<ui::font_atlas> const _font_atlas;
    std::shared_ptr<ui::layout_value_guide> const _top_guide;
    std::shared_ptr<ui::button> const _bg_button;
    std::shared_ptr<ui::node> const _buttons_root_node;
    std::vector<button_element> _button_elements;
    std::shared_ptr<ui::rect_plane> const _nudge_plane;
    std::shared_ptr<ui::strings> const _time_strings;

    observing::canceller_pool _pool;

    void _resize_buttons();
    void _update_button_positions();
    std::optional<ui::region> _button_region(index_range const &) const;
    void _update_unit_states();
    void _update_time_strings_attributes();
    void _update_nudge_position();
};
}  // namespace yas::ae
