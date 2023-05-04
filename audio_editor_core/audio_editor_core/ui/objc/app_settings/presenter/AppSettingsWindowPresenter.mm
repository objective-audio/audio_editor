//
//  AppSettingsWindowPresenter.mm
//

#import "AppSettingsWindowPresenter.h"
#import <audio_editor_core/ae_app_settings_lifetime_id+objc.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_app_settings_closer.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct app_settings_window_presenter_cpp {
    app_settings_window_presenter_cpp(std::shared_ptr<app_settings_closer> const &closer) : _closer(closer) {
    }

    [[nodiscard]] bool should_close() {
        auto const closer = this->_closer.lock();

        if (closer && closer->can_close()) {
            closer->close_if_needed();
            return true;
        } else {
            return false;
        }
    }

   private:
    std::weak_ptr<app_settings_closer> const _closer;
};
}

@implementation AppSettingsWindowPresenter {
    std::shared_ptr<app_settings_window_presenter_cpp> _cpp;
}

- (instancetype)initWithLifetimeId:(AppSettingsLifetimeId *)lifetimeId {
    self = [super init];
    if (self) {
        _lifetimeId = lifetimeId;

        auto const raw_lifetime_id = lifetimeId.raw_value;
        auto const &app_settings_lifetime = hierarchy::app_settings_lifetime_for_id(raw_lifetime_id);
        _cpp = std::make_unique<app_settings_window_presenter_cpp>(app_settings_lifetime->closer);
    }
    return self;
}

- (BOOL)shouldClose {
    return self->_cpp->should_close();
}

@end
