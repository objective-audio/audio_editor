#import "ProjectSubLifetimeId.h"
#include <ae-core/global/value_types/project_sub_lifetime_id.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct project_sub_lifetime_id_cpp {
    project_sub_lifetime_id const raw_value;

    project_sub_lifetime_id_cpp(project_sub_lifetime_id const raw_value) : raw_value(raw_value) {
    }
};
}  // namespace yas::ae

@implementation ProjectSubLifetimeId {
    std::unique_ptr<project_sub_lifetime_id_cpp> _cpp;
}

- (instancetype)initWithRawValue:(project_sub_lifetime_id const)raw_value {
    self = [super init];
    if (self) {
        _cpp = std::make_unique<project_sub_lifetime_id_cpp>(raw_value);
    }
    return self;
}

- (yas::ae::project_sub_lifetime_id)raw_value {
    return self->_cpp->raw_value;
}

- (BOOL)isEqual:(id)object {
    return [self raw_value] == [object raw_value];
}

@end
