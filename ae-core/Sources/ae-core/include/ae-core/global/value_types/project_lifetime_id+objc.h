//
//  project_lifetime_id+objc.h
//

#pragma once

#import <ae-core/global/value_types/ProjectLifetimeId.h>
#include <ae-core/global/value_types/project_lifetime_id.h>

@interface ProjectLifetimeId (Cpp)

- (instancetype)initWithRawValue:(yas::ae::project_lifetime_id const)raw_value;

- (yas::ae::project_lifetime_id)raw_value;

@end
