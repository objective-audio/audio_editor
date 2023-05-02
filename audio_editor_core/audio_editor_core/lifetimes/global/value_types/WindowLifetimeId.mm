//
//  WindowLifetimeId.mm
//

#import "WindowLifetimeId.h"
#include <audio_editor_core/ae_window_lifetime_id.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct window_lifetime_id_cpp {
    window_lifetime_id const raw_value;

    window_lifetime_id_cpp(window_lifetime_id const raw_value) : raw_value(raw_value) {
    }
};
}

@implementation WindowLifetimeId {
    std::unique_ptr<window_lifetime_id_cpp> _cpp;
}

- (instancetype)initWithRawValue:(window_lifetime_id const)raw_value {
    self = [super init];
    if (self) {
        _cpp = std::make_unique<window_lifetime_id_cpp>(raw_value);
    }
    return self;
}

- (yas::ae::window_lifetime_id)raw_value {
    return self->_cpp->raw_value;
}

@end
