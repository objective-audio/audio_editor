//
//  ae_color.h
//

#pragma once

#include <ui/yas_ui_color.h>

namespace yas::ae {
struct color final {
    color() = default;

    [[nodiscard]] ui::color background() const;

    [[nodiscard]] ui::color grid_line() const;

    [[nodiscard]] ui::color playing_line_pausing() const;
    [[nodiscard]] ui::color playing_line_playing() const;
    [[nodiscard]] ui::color playing_line_scrolling() const;

    [[nodiscard]] ui::color module_frame() const;
    [[nodiscard]] ui::color module_bg() const;
    [[nodiscard]] ui::color selected_module_bg() const;
    [[nodiscard]] ui::color module_name() const;
    [[nodiscard]] ui::color selected_module_name() const;
    [[nodiscard]] ui::color waveform() const;
    [[nodiscard]] ui::color selected_waveform() const;

    [[nodiscard]] ui::color range_selection_fill() const;
    [[nodiscard]] ui::color range_selection_frame() const;

    [[nodiscard]] ui::color pasting_module_frame() const;

    [[nodiscard]] ui::color edge_line() const;
    [[nodiscard]] ui::color edge_text() const;

    [[nodiscard]] ui::color marker_square() const;
    [[nodiscard]] ui::color selected_marker_square() const;
    [[nodiscard]] ui::color marker_line() const;
    [[nodiscard]] ui::color marker_text() const;
    [[nodiscard]] ui::color selected_marker_text() const;

    [[nodiscard]] ui::color pasting_marker_line() const;

    [[nodiscard]] ui::color track_square() const;
    [[nodiscard]] ui::color selected_track_square() const;
    [[nodiscard]] ui::color track_bg() const;
    [[nodiscard]] ui::color selected_track_bg() const;

    [[nodiscard]] ui::color modal_bg() const;

    [[nodiscard]] ui::color time_text() const;
    [[nodiscard]] ui::color time_selected_text() const;
    [[nodiscard]] ui::color time_nudging_line() const;
    [[nodiscard]] ui::color time_bg() const;
    [[nodiscard]] ui::color time_bg_tracking() const;
    [[nodiscard]] ui::color time_unit() const;
    [[nodiscard]] ui::color time_unit_tracking() const;
    [[nodiscard]] ui::color time_unit_selected() const;
    [[nodiscard]] ui::color time_unit_selected_tracking() const;

    [[nodiscard]] ui::color debug_text() const;

   private:
    color(color const &) = delete;
    color(color &&) = delete;
    color &operator=(color const &) = delete;
    color &operator=(color &&) = delete;
};
}  // namespace yas::ae
