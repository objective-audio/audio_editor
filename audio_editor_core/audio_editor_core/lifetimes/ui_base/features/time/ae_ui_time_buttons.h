//
//  ae_ui_time_buttons.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class time_numbers_presenter;
class project_action_sender;
class color;
class ui_time_numbers;

struct ui_time_buttons final {
    [[nodiscard]] static std::shared_ptr<ui_time_buttons> make_shared(window_lifetime_id const &, ui::node *node,
                                                                      ui_time_numbers *time_numbers);

    ui_time_buttons(std::shared_ptr<ui::standard> const &, ae::color *, std::shared_ptr<time_numbers_presenter> const &,
                    std::shared_ptr<project_action_sender> const &, ui::node *node, ui_time_numbers *time_numbers);

   private:
    struct button_element {
        std::shared_ptr<ui::button> button;
        observing::cancellable_ptr canceller;
    };

    std::shared_ptr<time_numbers_presenter> const _presenter;
    std::weak_ptr<project_action_sender> const _action_sender;
    std::weak_ptr<ui::standard> const _standard;
    ae::color *const _color;
    ui::node *const _node;
    std::vector<button_element> _button_elements;
    ui_time_numbers *const _time_numbers;

    observing::canceller_pool _pool;

    ui_time_buttons(ui_time_buttons const &) = delete;
    ui_time_buttons(ui_time_buttons &&) = delete;
    ui_time_buttons &operator=(ui_time_buttons const &) = delete;
    ui_time_buttons &operator=(ui_time_buttons &&) = delete;

    void _resize_buttons();
    void _update_button_positions();
    void _update_unit_states();
};
}  // namespace yas::ae
