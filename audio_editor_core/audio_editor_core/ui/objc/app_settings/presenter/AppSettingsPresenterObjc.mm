//
//  AppSettingsPresenterObjc.mm
//

#import "AppSettingsPresenterObjc.h"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_timing_types+objc.h>
#include <audio_editor_core/ae_app_settings.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct app_settings_presenter_cpp final {
    std::weak_ptr<app_settings> const app_settings;
    observing::canceller_pool cancellers;

    app_settings_presenter_cpp(std::shared_ptr<ae::app_settings> const &app_settings) : app_settings(app_settings) {
    }
};
}

@implementation AppSettingsPresenterObjc {
    std::unique_ptr<app_settings_presenter_cpp> _cpp;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        auto const &app_lifetime = hierarchy::app_lifetime();
        self->_cpp = std::make_unique<app_settings_presenter_cpp>(app_lifetime->app_settings);
    }
    return self;
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
