//
//  ae_color.h
//

#pragma once

#include <ui/yas_ui_color.h>

namespace yas::ae {
struct color final {
    static std::shared_ptr<color> make_shared();

    ui::color background() const;

    ui::color playing_line_pausing() const;
    ui::color playing_line_playing() const;
    ui::color playing_line_scroling() const;

    ui::color time_text() const;
    ui::color time_selected_text() const;
    ui::color time_nudging_line() const;
    ui::color time_bg() const;
    ui::color time_bg_tracking() const;
    ui::color time_unit() const;
    ui::color time_unit_tracking() const;
    ui::color time_unit_selected() const;
    ui::color time_unit_selected_tracking() const;

    ui::color debug_text() const;

   private:
    color();
};
}  // namespace yas::ae
