//
//  color.mm
//

#include "color.h"
#import <AppKit/AppKit.h>
#import <ae-bundle/AEBundle.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
static ui::color to_color(NSString *name) {
    @autoreleasepool {
        NSColor *nsColor = [AEBundle colorWithName:name];

        if (!nsColor) {
            throw std::runtime_error("color not found.");
        }

        return {.red = static_cast<float>(nsColor.redComponent),
                .green = static_cast<float>(nsColor.greenComponent),
                .blue = static_cast<float>(nsColor.blueComponent),
                .alpha = static_cast<float>(nsColor.alphaComponent)};
    }
}
}  // namespace yas::ae

ui::color color::background() const {
    return to_color(@"background");
}

ui::color color::grid_line() const {
    return to_color(@"grid_line");
}

ui::color color::playing_line_pausing() const {
    return to_color(@"playing_line_pausing");
}

ui::color color::playing_line_playing() const {
    return to_color(@"playing_line_playing");
}

ui::color color::playing_line_scrolling() const {
    return to_color(@"playing_line_scrolling");
}

ui::color color::module_frame() const {
    return to_color(@"module_frame");
}

ui::color color::module_bg() const {
    return to_color(@"module_bg");
}

ui::color color::selected_module_bg() const {
    return to_color(@"selected_module_bg");
}

ui::color color::module_name() const {
    return to_color(@"module_name");
}

ui::color color::selected_module_name() const {
    return to_color(@"selected_module_name");
}

ui::color color::waveform() const {
    return to_color(@"waveform");
}

ui::color color::selected_waveform() const {
    return to_color(@"selected_waveform");
}

ui::color color::range_selection_fill() const {
    return to_color(@"range_selection_fill");
}

ui::color color::range_selection_frame() const {
    return to_color(@"range_selection_frame");
}

ui::color color::pasting_module_frame() const {
    return to_color(@"pasting_module_frame");
}

ui::color color::edge_line() const {
    return to_color(@"edge_line");
}

ui::color color::edge_text() const {
    return to_color(@"edge_text");
}

ui::color color::marker_square() const {
    return to_color(@"marker_square");
}

ui::color color::selected_marker_square() const {
    return to_color(@"selected_marker_square");
}

ui::color color::marker_line() const {
    return to_color(@"marker_line");
}

ui::color color::marker_text() const {
    return to_color(@"marker_text");
}

ui::color color::selected_marker_text() const {
    return to_color(@"selected_marker_text");
}

ui::color color::pasting_marker_line() const {
    return to_color(@"pasting_marker_line");
}

ui::color color::track_square() const {
    return to_color(@"track_square");
}

ui::color color::selected_track_square() const {
    return to_color(@"selected_track_square");
}

ui::color color::track_bg() const {
    return to_color(@"track_bg");
}

ui::color color::selected_track_bg() const {
    return to_color(@"selected_track_bg");
}

ui::color color::track_text() const {
    return to_color(@"track_text");
}

ui::color color::selected_track_text() const {
    return to_color(@"selected_track_text");
}

ui::color color::modal_bg() const {
    return to_color(@"modal_bg");
}

ui::color color::time_text() const {
    return to_color(@"time_text");
}

ui::color color::time_selected_text() const {
    return to_color(@"time_selected_text");
}

ui::color color::time_nudging_line() const {
    return to_color(@"time_nudging_line");
}

ui::color color::time_bg() const {
    return to_color(@"time_bg");
}

ui::color color::time_bg_tracking() const {
    return to_color(@"time_bg_tracking");
}

ui::color color::time_unit() const {
    return to_color(@"time_unit");
}

ui::color color::time_unit_tracking() const {
    return to_color(@"time_unit_tracking");
}

ui::color color::time_unit_selected() const {
    return to_color(@"time_unit_selected");
}

ui::color color::time_unit_selected_tracking() const {
    return to_color(@"time_unit_selected_tracking");
}

ui::color color::debug_text() const {
    return to_color(@"debug_text");
}
