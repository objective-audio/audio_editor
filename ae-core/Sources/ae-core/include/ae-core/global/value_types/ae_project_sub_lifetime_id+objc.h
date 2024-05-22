#pragma once

#import <ae-core/global/value_types/ProjectSubLifetimeId.h>
#include <ae-core/global/value_types/ae_project_sub_lifetime_id.h>

@interface ProjectSubLifetimeId (Cpp)

- (instancetype)initWithRawValue:(yas::ae::project_sub_lifetime_id const)raw_value;

- (yas::ae::project_sub_lifetime_id)raw_value;

@end
