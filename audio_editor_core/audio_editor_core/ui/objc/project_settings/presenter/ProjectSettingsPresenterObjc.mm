//
//  ProjectSettingsPresenterObjc.mm
//

#import "ProjectSettingsPresenterObjc.h"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_timing_types+objc.h>
#include <audio_editor_core/ae_window_lifetime_id+objc.h>
#include <audio_editor_core/ae_app_settings.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct project_settings_presenter_cpp final {
    window_lifetime_id const lifetime_id;
    project_format const project_format;
    observing::canceller_pool cancellers;

    project_settings_presenter_cpp(window_lifetime_id const lifetime_id, ae::project_format const &project_format)
        : lifetime_id(lifetime_id), project_format(project_format) {
    }
};
}

@implementation ProjectSettingsPresenterObjc {
    std::unique_ptr<project_settings_presenter_cpp> _cpp;
}

- (instancetype)initWithWindowLifetimeId:(WindowLifetimeId *)lifetimeId {
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
