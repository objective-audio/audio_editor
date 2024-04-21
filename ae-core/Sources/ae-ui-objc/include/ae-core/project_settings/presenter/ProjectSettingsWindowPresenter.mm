//
//  ProjectSettingsWindowPresenter.mm
//

#import "ProjectSettingsWindowPresenter.h"
#include <ae-core/global/ae_hierarchy.h>
#import <ae-core/global/value_types/ae_project_lifetime_id+objc.h>
#include <ae-core/project_settings/features/ae_project_settings_closer.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct project_settings_window_presenter_cpp {
    project_settings_window_presenter_cpp(project_lifetime_id const lifetime_id,
                                          std::shared_ptr<project_settings_closer> const &closer)
        : lifetime_id(lifetime_id), _closer(closer) {
    }

    project_lifetime_id const lifetime_id;

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
    std::weak_ptr<project_settings_closer> const _closer;
};
}  // namespace yas::ae

@implementation ProjectSettingsWindowPresenter {
    std::unique_ptr<project_settings_window_presenter_cpp> _cpp;
}

- (instancetype)initWithLifetimeId:(ProjectLifetimeId *)lifetimeId {
    self = [super init];
    if (self) {
        _lifetimeId = lifetimeId;

        auto const raw_lifetime_id = lifetimeId.raw_value;
        auto const &settings_lifetime = hierarchy::project_settings_lifetime_for_id(raw_lifetime_id);
        _cpp = std::make_unique<project_settings_window_presenter_cpp>(raw_lifetime_id, settings_lifetime->closer);
    }
    return self;
}

- (BOOL)shouldClose {
    return self->_cpp->should_close();
}

@end
