//
//  ProjectSettingsPresenterObjc.mm
//

#import "ProjectSettingsPresenterObjc.h"
#include <ae-core/global/ae_hierarchy.h>
#include <ae-core/global/value_types/ae_project_lifetime_id+objc.h>
#include <ae-core/project_editing/value_types/ae_timing_types+objc.h>
#include <ae-core/app/features/ae_app_settings.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct project_settings_presenter_cpp final {
    project_lifetime_id const lifetime_id;
    project_format const project_format;
    observing::canceller_pool cancellers;

    project_settings_presenter_cpp(project_lifetime_id const lifetime_id, ae::project_format const &project_format)
        : lifetime_id(lifetime_id), project_format(project_format) {
    }
};
}  // namespace yas::ae

@implementation ProjectSettingsPresenterObjc {
    std::unique_ptr<project_settings_presenter_cpp> _cpp;
}

- (instancetype)initWithProjectLifetimeId:(ProjectLifetimeId *)lifetimeId {
    self = [super init];
    if (self) {
        auto const raw_lifetime_id = lifetimeId.raw_value;
        auto const &settings_lifetime = hierarchy::project_settings_lifetime_for_id(raw_lifetime_id);
        self->_cpp =
            std::make_unique<project_settings_presenter_cpp>(raw_lifetime_id, settings_lifetime->project_format);
    }
    return self;
}

- (NSInteger)sampleRate {
    return self->_cpp->project_format.sample_rate;
}

- (NSInteger)channelCount {
    return self->_cpp->project_format.channel_count;
}

@end
