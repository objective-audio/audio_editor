#import "AppWindowPresenterObjc.h"

#include <ae-core/app/features/ae_project_opener.h>
#include <ae-core/app/lifecycles/ae_app_modal_lifecycle.h>
#include <ae-core/app/lifecycles/ae_project_lifecycle.h>
#include <ae-core/global/ae_hierarchy.h>
#import <ae-core/global/value_types/ae_app_settings_lifetime_id+objc.h>
#import <ae-core/global/value_types/ae_project_lifetime_id+objc.h>
#include <ae-core/project_setup/presenter/ae_project_setup_presenter.h>
#include <cpp-utils/cf_utils.h>
#include <cpp-utils/unowned.h>
#include <ae-core/app/features/ae_app_settings_opener.hpp>
#include <ae-core/app/features/ae_project_setup_dialog_opener.hpp>
#include <ae-core/app/lifecycles/ae_app_settings_lifecycle.hpp>
#include <ae-core/app/lifecycles/ae_project_settings_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct app_window_presenter_cpp final {
    std::weak_ptr<app_settings_opener> const app_settings_opener;
    std::weak_ptr<project_setup_dialog_opener> const project_setup_dialog_opener;
    std::weak_ptr<project_opener> const project_opener;

    std::weak_ptr<project_lifecycle> const project_lifecycle;
    std::weak_ptr<project_settings_lifecycle> const project_settings_lifecycle;
    std::weak_ptr<app_settings_lifecycle> const app_settings_lifecycle;
    std::weak_ptr<app_modal_lifecycle> const app_modal_lifecycle;

    observing::canceller_pool cancellers;

    app_window_presenter_cpp(std::shared_ptr<ae::app_settings_opener> const &app_settings_opener,
                             std::shared_ptr<ae::project_setup_dialog_opener> const &project_setup_dialog_opener,
                             std::shared_ptr<ae::project_opener> const &project_opener,
                             std::shared_ptr<ae::project_lifecycle> const &project_lifecycle,
                             std::shared_ptr<ae::project_settings_lifecycle> const &project_settings_lifecycle,
                             std::shared_ptr<ae::app_settings_lifecycle> const &app_settings_lifecycle,
                             std::shared_ptr<ae::app_modal_lifecycle> const &app_modal_lifecycle)
        : app_settings_opener(app_settings_opener),
          project_setup_dialog_opener(project_setup_dialog_opener),
          project_opener(project_opener),
          project_lifecycle(project_lifecycle),
          project_settings_lifecycle(project_settings_lifecycle),
          app_settings_lifecycle(app_settings_lifecycle),
          app_modal_lifecycle(app_modal_lifecycle) {
    }
};
}  // namespace yas::ae

@implementation AppWindowPresenterObjc {
    std::unique_ptr<app_window_presenter_cpp> _cpp;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        auto const &app_lifetime = hierarchy::app_lifetime();
        auto const &project_opener = app_lifetime->project_opener;
        auto const &project_lifecycle = app_lifetime->project_lifecycle;
        auto const &project_settings_lifecycle = app_lifetime->project_settings_lifecycle;
        auto const &app_settings_lifecycle = app_lifetime->app_settings_lifecycle;
        auto const &modal_lifecycle = app_lifetime->modal_lifecycle;

        _cpp = std::make_unique<app_window_presenter_cpp>(
            app_lifetime->app_settings_opener, app_lifetime->project_setup_dialog_opener, project_opener,
            project_lifecycle, project_settings_lifecycle, app_settings_lifecycle, modal_lifecycle);

        auto *const unowned = make_unowned(self);

        project_opener
            ->observe_event([unowned](project_opener_event const &event) {
                auto const lifetimeId = [[ProjectLifetimeId alloc] initWithRawValue:event.lifetime_id];

                switch (event.kind) {
                    case project_opener_event_kind::show_opened:
                        [unowned.object showProjectWithLifetimeId:lifetimeId];
                        break;
                }
            })
            .end()
            ->add_to(self->_cpp->cancellers);

        project_lifecycle
            ->observe_event([unowned](project_lifecycle_event const &event) {
                auto const lifetimeId = [[ProjectLifetimeId alloc] initWithRawValue:event.lifetime_id];

                switch (event.type) {
                    case project_lifecycle_event_type::inserted:
                        [unowned.object makeAndShowProjectWithLifetimeId:lifetimeId];
                        break;
                    case project_lifecycle_event_type::erased:
                        [unowned.object disposeProjectWithLifetimeId:lifetimeId];
                        break;
                }
            })
            .sync()
            ->add_to(self->_cpp->cancellers);

        project_settings_lifecycle
            ->observe_event([unowned](project_settings_lifecycle_event const &event) {
                auto const lifetimeId = [[ProjectLifetimeId alloc] initWithRawValue:event.lifetime_id];

                switch (event.type) {
                    case project_settings_lifecycle_event_type::inserted:
                        [unowned.object makeAndShowProjectSettingsWithLifetimeId:lifetimeId];
                        break;
                    case project_settings_lifecycle_event_type::erased:
                        [unowned.object disposeProjectSettingsWithLifetimeId:lifetimeId];
                        break;
                }
            })
            .sync()
            ->add_to(self->_cpp->cancellers);

        app_settings_lifecycle
            ->observe_event([unowned](app_settings_lifecycle_event const &event) {
                auto const lifetimeId = [[AppSettingsLifetimeId alloc] initWithRawValue:event.lifetime_id];

                switch (event.type) {
                    case app_settings_lifecycle_event_type::inserted:
                        [unowned.object makeAndShowAppSettingsWithLifetimeId:lifetimeId];
                        break;
                    case app_settings_lifecycle_event_type::erased:
                        [unowned.object disposeAppSettingsWithLifetimeId:lifetimeId];
                        break;
                }
            })
            .sync()
            ->add_to(self->_cpp->cancellers);

        modal_lifecycle
            ->observe([unowned](std::optional<app_modal_sub_lifetime> const &sub_lifetime) {
                switch (to_kind(sub_lifetime)) {
                    case app_modal_sub_lifetime_kind::none:
                        // panelは強制的に閉じれないので何もしない
                        break;
                    case app_modal_sub_lifetime_kind::project_setup_dialog:
                        [unowned.object openProjectSetupDialog];
                        break;
                }
            })
            .sync()
            ->add_to(self->_cpp->cancellers);
    }
    return self;
}

- (BOOL)respondsToAction:(AppWindowAction const)action {
    switch (action) {
        case AppWindowActionOpenProject: {
            if (auto const opener = self->_cpp->project_setup_dialog_opener.lock()) {
                return opener->can_open();
            } else {
                return NO;
            }
        }
        case AppWindowActionOpenAppSettings: {
            if (auto const opener = self->_cpp->app_settings_opener.lock()) {
                return opener->can_open();
            } else {
                return NO;
            }
        }
        case AppWindowActionUnknown: {
            return NO;
        }
    }
}

- (void)performAction:(AppWindowAction const)action {
    switch (action) {
        case AppWindowActionOpenProject: {
            if (auto const opener = self->_cpp->project_setup_dialog_opener.lock()) {
                opener->open();
            }
        } break;
        case AppWindowActionOpenAppSettings: {
            if (auto const opener = self->_cpp->app_settings_opener.lock()) {
                opener->open();
            }
        } break;
        case AppWindowActionUnknown: {
            // do nothing.
        } break;
    }
}

- (void)openProjectSetupDialog {
    auto const presenter = project_setup_presenter::make_shared();
    auto const unowned_self = make_unowned(self);

    [self openProjectSetupDialogWithCompletion:[unowned_self, presenter](NSModalResponse result, NSURL *_Nonnull url) {
        if (result == NSModalResponseOK) {
            // SwiftのURLのpath()だと最後の要素が変わるのでNSURLのpathで取得している
            auto const file_path = to_string((__bridge CFStringRef)url.path);
            presenter->select_directory(file_path);
        }

        presenter->did_close_dialog();
    }];
}

- (void)openProjectSetupDialogWithCompletion:(void (^)(NSModalResponse, NSURL *))completion {
    NSAssert(NO, @"should be overridden.");
}

- (void)makeAndShowProjectSettingsWithLifetimeId:(ProjectLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden.");
}

- (void)disposeProjectSettingsWithLifetimeId:(ProjectLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden.");
}

- (void)makeAndShowAppSettingsWithLifetimeId:(AppSettingsLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden.");
}

- (void)disposeAppSettingsWithLifetimeId:(AppSettingsLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden.");
}

- (void)makeAndShowProjectWithLifetimeId:(ProjectLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden.");
}

- (void)showProjectWithLifetimeId:(ProjectLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden.");
}

- (void)disposeProjectWithLifetimeId:(ProjectLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden.");
}

@end
