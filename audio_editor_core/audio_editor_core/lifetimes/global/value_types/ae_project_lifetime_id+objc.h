//
//  ae_project_lifetime_id+objc.h
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>

#import <audio_editor_core/ProjectLifetimeId.h>

@interface ProjectLifetimeId (Cpp)

- (instancetype)initWithRawValue:(yas::ae::project_lifetime_id const)raw_value;

- (yas::ae::project_lifetime_id)raw_value;

@end
