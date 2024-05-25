#import "AppSettingsLifetimeId.h"

#include <ae-core/global/value_types/app_settings_lifetime_id.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct app_settings_lifetime_id_cpp {
    app_settings_lifetime_id const raw_value;

    app_settings_lifetime_id_cpp(app_settings_lifetime_id const raw_value) : raw_value(raw_value) {
    }
};
}  // namespace yas::ae

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

- (BOOL)isEqual:(id)object {
    return [self raw_value] == [object raw_value];
}

@end
