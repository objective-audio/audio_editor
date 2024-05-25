//
//  app_settings_lifetime_id+objc.h
//

#pragma once

#import <ae-core/global/value_types/AppSettingsLifetimeId.h>
#include <ae-core/global/value_types/app_settings_lifetime_id.hpp>

@interface AppSettingsLifetimeId (Cpp)

- (instancetype)initWithRawValue:(yas::ae::app_settings_lifetime_id const)raw_value;

- (yas::ae::app_settings_lifetime_id)raw_value;

@end
