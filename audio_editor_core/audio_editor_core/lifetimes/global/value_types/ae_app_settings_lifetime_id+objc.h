//
//  ae_app_settings_lifetime_id+objc.h
//

#pragma once

#include <audio_editor_core/ae_app_settings_lifetime_id.hpp>

#import <audio_editor_core/AppSettingsLifetimeId.h>

@interface AppSettingsLifetimeId (Cpp)

- (instancetype)initWithRawValue:(yas::ae::app_settings_lifetime_id const)raw_value;

- (yas::ae::app_settings_lifetime_id)raw_value;

@end
