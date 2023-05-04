//
//  AppSettingsLifetimeId.mm
//

#import "AppSettingsLifetimeId.h"

#include <audio_editor_core/ae_app_settings_lifetime_id.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct app_settings_lifetime_id_cpp {
    app_settings_lifetime_id const raw_value;

    app_settings_lifetime_id_cpp(app_settings_lifetime_id const raw_value) : raw_value(raw_value) {
    }
};
}

@implementation AppSettingsLifetimeId {
    std::unique_ptr<app_settings_lifetime_id_cpp> _cpp;
}

- (instancetype)initWithRawValue:(app_settings_lifetime_id const)raw_value {
    self = [super init];
    if (self) {
        _cpp = std::make_unique<app_settings_lifetime_id_cpp>(raw_value);
    }
    return self;
}

- (yas::ae::app_settings_lifetime_id)raw_value {
    return self->_cpp->raw_value;
}

@end
