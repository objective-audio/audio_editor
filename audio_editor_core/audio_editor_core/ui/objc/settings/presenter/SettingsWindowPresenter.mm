//
//  SettingsWindowPresenter.mm
//

#import "SettingsWindowPresenter.h"
#include <audio_editor_core/ae_hierarchy.h>
#import <audio_editor_core/ae_window_lifetime_id+objc.h>
#include <audio_editor_core/ae_settings_closer.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct settings_window_presenter_cpp {
    settings_window_presenter_cpp(window_lifetime_id const lifetime_id, std::shared_ptr<settings_closer> const &closer)
        : lifetime_id(lifetime_id), _closer(closer) {
    }

    window_lifetime_id const lifetime_id;

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
    std::weak_ptr<settings_closer> const _closer;
};
}

@implementation SettingsWindowPresenter {
    std::shared_ptr<settings_window_presenter_cpp> _cpp;
}

- (instancetype)initWithLifetimeId:(WindowLifetimeId *)lifetimeId {
    self = [super init];
    if (self) {
        _lifetimeId = lifetimeId;

        auto const raw_lifetime_id = lifetimeId.raw_value;
        auto const &settings_lifetime = hierarchy::settings_lifetime_for_id(raw_lifetime_id);
        _cpp = std::make_shared<settings_window_presenter_cpp>(raw_lifetime_id, settings_lifetime->closer);
    }
    return self;
}

- (BOOL)shouldClose {
    return self->_cpp->should_close();
}

@end
