//
//  SettingsPresenterObjc.mm
//

#import "SettingsPresenterObjc.h"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_timing_types+objc.h>
#include <audio_editor_core/ae_window_lifetime_id+objc.h>
#include <audio_editor_core/ae_app_settings.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct settings_presenter_cpp final {
    window_lifetime_id const lifetime_id;
    project_format const project_format;
    std::weak_ptr<app_settings> const app_settings;
    observing::canceller_pool cancellers;

    settings_presenter_cpp(window_lifetime_id const lifetime_id, ae::project_format const &project_format,
                           std::shared_ptr<ae::app_settings> const &app_settings)
        : lifetime_id(lifetime_id), project_format(project_format), app_settings(app_settings) {
    }
};
}

@implementation SettingsPresenterObjc {
    std::unique_ptr<settings_presenter_cpp> _cpp;
}

- (instancetype)initWithWindowLifetimeId:(WindowLifetimeId *)lifetimeId {
    self = [super init];
    if (self) {
        auto const raw_lifetime_id = lifetimeId.raw_value;
        auto const &app_lifetime = hierarchy::app_lifetime();
        auto const &settings_lifetime = hierarchy::settings_lifetime_for_id(raw_lifetime_id);
        self->_cpp = std::make_unique<settings_presenter_cpp>(raw_lifetime_id, settings_lifetime->project_format,
                                                              app_lifetime->app_settings);
    }
    return self;
}

- (NSInteger)sampleRate {
    return self->_cpp->project_format.sample_rate;
}

- (NSInteger)channelCount {
    return self->_cpp->project_format.channel_count;
}

- (void)setTimingFractionKind:(TimingFractionKind)kind {
    if (auto const app_settings = self->_cpp->app_settings.lock()) {
        app_settings->set_timing_fraction_kind(to_cpp(kind));
    }
}

- (TimingFractionKind)timingFractionKind {
    if (auto const app_settings = self->_cpp->app_settings.lock()) {
        return to_objc(app_settings->timing_fraction_kind());
    } else {
        return TimingFractionKindSample;
    }
}

- (void)setTimingUnitKind:(TimingUnitKind)kind {
    if (auto const app_settings = self->_cpp->app_settings.lock()) {
        app_settings->set_timing_unit_kind(to_cpp(kind));
    }
}

- (TimingUnitKind)timingUnitKind {
    if (auto const app_settings = self->_cpp->app_settings.lock()) {
        return to_objc(app_settings->timing_unit_kind());
    } else {
        return TimingUnitKindFraction;
    }
}

- (void)observe:(void (^)())handler {
    if (auto const app_settings = self->_cpp->app_settings.lock()) {
        auto syncable =
            app_settings->observe_timing_fraction_kind([handler](ae::timing_fraction_kind const &) { handler(); });
        auto unit_syncable =
            app_settings->observe_timing_unit_kind([handler](ae::timing_unit_kind const &) { handler(); });

        syncable.merge(std::move(unit_syncable));

        syncable.sync()->add_to(self->_cpp->cancellers);
    }
}

@end
