//
//  ae_color.mm
//

#include "ae_color.h"
#import <AppKit/AppKit.h>
#include <iostream>

@interface AudioEditorCoreDummyClassForBundle : NSObject
@end
@implementation AudioEditorCoreDummyClassForBundle
@end

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
static ui::color to_color(NSString *name) {
    static NSBundle *const bundle = [NSBundle bundleForClass:[AudioEditorCoreDummyClassForBundle class]];

    @autoreleasepool {
        NSColor *nsColor = [[NSColor colorNamed:name
                                         bundle:bundle] colorUsingColorSpace:[NSColorSpace deviceRGBColorSpace]];

        if (!nsColor) {
            throw std::runtime_error("color not found.");
        }

        return {.red = static_cast<float>(nsColor.redComponent),
                .green = static_cast<float>(nsColor.greenComponent),
                .blue = static_cast<float>(nsColor.blueComponent),
                .alpha = static_cast<float>(nsColor.alphaComponent)};
    }
}
}

std::shared_ptr<color> color::make_shared() {
    return std::shared_ptr<color>(new color{});
}

color::color() {
}

ui::color color::background() const {
    return to_color(@"background");
}

ui::color color::playing_line_pausing() const {
    return to_color(@"playing_line_pausing");
}

ui::color color::playing_line_playing() const {
    return to_color(@"playing_line_playing");
}

ui::color color::playing_line_scroling() const {
    return to_color(@"playing_line_playing");
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
