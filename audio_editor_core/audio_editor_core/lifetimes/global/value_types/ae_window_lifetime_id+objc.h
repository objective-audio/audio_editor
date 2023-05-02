//
//  ae_window_lifetime_id+objc.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

#import <audio_editor_core/WindowLifetimeId.h>

@interface WindowLifetimeId (Cpp)

- (instancetype)initWithRawValue:(yas::ae::window_lifetime_id const)raw_value;

- (yas::ae::window_lifetime_id)raw_value;

@end
