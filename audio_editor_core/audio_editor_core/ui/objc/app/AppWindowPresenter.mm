//
//  AppWindowPresenter.mm
//

#import "AppWindowPresenter.h"

#import <audio_editor_core/AEProjectFormatView.h>
#import <audio_editor_core/ProjectWindowController.h>
#include <audio_editor_core/ae_app_modal_lifecycle.h>
#import <audio_editor_core/ae_app_settings_lifetime_id+objc.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#import <audio_editor_core/ae_project_lifetime_id+objc.h>
#include <audio_editor_core/ae_project_setup_presenter.h>
#import <audio_editor_core/audio_editor_core-Swift.h>
#include <cpp_utils/yas_cf_utils.h>
#include <cpp_utils/yas_unowned.h>
#include <audio_editor_core/ae_app_settings_lifecycle.hpp>
#include <audio_editor_core/ae_app_settings_opener.hpp>
#include <audio_editor_core/ae_project_settings_lifecycle.hpp>
#include <audio_editor_core/ae_project_setup_dialog_opener.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct app_window_presenter_cpp final {
    std::weak_ptr<app_settings_opener> const app_settings_opener;
    std::weak_ptr<project_setup_dialog_opener> const project_setup_dialog_opener;

    std::weak_ptr<project_lifecycle> const project_lifecycle;
    std::weak_ptr<project_settings_lifecycle> const project_settings_lifecycle;
    std::weak_ptr<app_settings_lifecycle> const app_settings_lifecycle;
    std::weak_ptr<app_modal_lifecycle> const app_modal_lifecycle;

    observing::canceller_pool cancellers;

    app_window_presenter_cpp(std::shared_ptr<ae::app_settings_opener> const &app_settings_opener,
                             std::shared_ptr<ae::project_setup_dialog_opener> const &project_setup_dialog_opener,
                             std::shared_ptr<ae::project_lifecycle> const &project_lifecycle,
                             std::shared_ptr<ae::project_settings_lifecycle> const &project_settings_lifecycle,
                             std::shared_ptr<ae::app_settings_lifecycle> const &app_settings_lifecycle,
                             std::shared_ptr<ae::app_modal_lifecycle> const &app_modal_lifecycle)
        : app_settings_opener(app_settings_opener),
          project_setup_dialog_opener(project_setup_dialog_opener),
          project_lifecycle(project_lifecycle),
          project_settings_lifecycle(project_settings_lifecycle),
          app_settings_lifecycle(app_settings_lifecycle),
          app_modal_lifecycle(app_modal_lifecycle) {
    }
};
}

@interface AppWindowPresenter ()

@property (nonatomic) NSMutableSet<ProjectWindowController *> *projectControllers;
@property (nonatomic) NSMutableSet<ProjectSettingsWindowController *> *projectSettingsControllers;
@property (nonatomic) NSMutableSet<AppSettingsWindowController *> *appSettingsControllers;

@end

@implementation AppWindowPresenter {
    std::unique_ptr<app_window_presenter_cpp> _cpp;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        self.projectControllers = [[NSMutableSet alloc] init];
        self.projectSettingsControllers = [[NSMutableSet alloc] init];
        self.appSettingsControllers = [[NSMutableSet alloc] init];

        auto const &app_lifetime = hierarchy::app_lifetime();
        auto const &project_lifecycle = app_lifetime->project_lifecycle;
        auto const &project_settings_lifecycle = app_lifetime->project_settings_lifecycle;
        auto const &app_settings_lifecycle = app_lifetime->app_settings_lifecycle;
        auto const &modal_lifecycle = app_lifetime->modal_lifecycle;

        _cpp = std::make_unique<app_window_presenter_cpp>(
            app_lifetime->app_settings_opener, app_lifetime->project_setup_dialog_opener, project_lifecycle,
            project_settings_lifecycle, app_settings_lifecycle, modal_lifecycle);

        auto *const unowned = make_unowned(self);

        project_lifecycle
            ->observe_event([unowned](project_lifecycle_event const &event) {
                switch (event.type) {
                    case project_lifecycle_event_type::inserted:
                        [unowned.object makeAndShowProjectWithLifetimeID:event.lifetime_id];
                        break;
                    case project_lifecycle_event_type::erased:
                        [unowned.object disposeProjectWithLifetimeID:event.lifetime_id];
                        break;
                }
            })
            .sync()
            ->add_to(self->_cpp->cancellers);

        project_settings_lifecycle
            ->observe_event([unowned](project_settings_lifecycle_event const &event) {
                switch (event.type) {
                    case project_settings_lifecycle_event_type::inserted:
                        [unowned.object makeAndShowProjectSettingsWithLifetimeID:event.lifetime_id];
                        break;
                    case project_settings_lifecycle_event_type::erased:
                        [unowned.object disposeProjectSettingsWithLifetimeID:event.lifetime_id];
                        break;
                }
            })
            .sync()
            ->add_to(self->_cpp->cancellers);

        app_settings_lifecycle
            ->observe_event([unowned](app_settings_lifecycle_event const &event) {
                switch (event.type) {
                    case app_settings_lifecycle_event_type::inserted:
                        [unowned.object makeAndShowAppSettingsWithLifetimeID:event.lifetime_id];
                        break;
                    case app_settings_lifecycle_event_type::erased:
                        [unowned.object disposeAppSettingsWithLifetimeID:event.lifetime_id];
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

- (BOOL)respondsToAction:(app_window_action_kind const)action_kind {
    switch (action_kind) {
        case app_window_action_kind::open_project: {
            if (auto const opener = self->_cpp->project_setup_dialog_opener.lock()) {
                return opener->can_open();
            } else {
                return NO;
            }
        }
        case app_window_action_kind::open_app_settings: {
            if (auto const opener = self->_cpp->app_settings_opener.lock()) {
                return opener->can_open();
            } else {
                return NO;
            }
        }
    }
}

- (void)performAction:(app_window_action_kind)action_kind {
    switch (action_kind) {
        case app_window_action_kind::open_project: {
            if (auto const opener = self->_cpp->project_setup_dialog_opener.lock()) {
                opener->open();
            }
        } break;
        case app_window_action_kind::open_app_settings: {
            if (auto const opener = self->_cpp->app_settings_opener.lock()) {
                opener->open();
            }
        } break;
    }
}

- (void)openProjectSetupDialog {
    auto const presenter = project_setup_presenter::make_shared();

    NSOpenPanel *panel = [NSOpenPanel openPanel];
    panel.allowsMultipleSelection = NO;
    panel.canChooseFiles = NO;
    panel.canChooseDirectories = YES;
    panel.canCreateDirectories = YES;
    panel.accessoryView = [AEProjectFormatView instantiate];

    auto const unowned_self = make_unowned(self);
    auto const unowned_panel = make_unowned(panel);

    [panel beginWithCompletionHandler:[unowned_self, unowned_panel, presenter](NSModalResponse result) {
        if (result == NSModalResponseOK) {
            auto const file_path = to_string((__bridge CFStringRef)unowned_panel.object.URL.path);
            presenter->select_directory(file_path);
        }

        presenter->did_close_dialog();
    }];
}

- (void)makeAndShowProjectWithLifetimeID:(project_lifetime_id const &)lifetime_id {
    ProjectWindowController *windowController = [ProjectWindowController instantiateWithLifetimeID:lifetime_id];
    [self.projectControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)disposeProjectWithLifetimeID:(project_lifetime_id const &)lifetime_id {
    NSMutableSet<ProjectWindowController *> *copiedWindowControllers = [self.projectControllers mutableCopy];

    for (ProjectWindowController *windowController in self.projectControllers) {
        if (windowController.lifetime_id == lifetime_id) {
            [windowController close];
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.projectControllers = copiedWindowControllers;
}

- (void)makeAndShowProjectSettingsWithLifetimeID:(project_lifetime_id const &)lifetime_id {
    ProjectSettingsWindowController *windowController = [ProjectSettingsWindowController
        instantiateWithLifetimeId:[[ProjectLifetimeId alloc] initWithRawValue:lifetime_id]];
    [self.projectSettingsControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)disposeProjectSettingsWithLifetimeID:(project_lifetime_id const &)lifetime_id {
    NSMutableSet<ProjectSettingsWindowController *> *copiedWindowControllers =
        [self.projectSettingsControllers mutableCopy];

    for (ProjectSettingsWindowController *windowController in self.projectSettingsControllers) {
        if ([windowController.lifetimeId raw_value] == lifetime_id) {
            [windowController close];
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.projectSettingsControllers = copiedWindowControllers;
}

- (void)makeAndShowAppSettingsWithLifetimeID:(app_settings_lifetime_id const &)lifetime_id {
    AppSettingsWindowController *windowController = [AppSettingsWindowController
        instantiateWithLifetimeId:[[AppSettingsLifetimeId alloc] initWithRawValue:lifetime_id]];
    [self.appSettingsControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)disposeAppSettingsWithLifetimeID:(app_settings_lifetime_id const &)lifetime_id {
    NSMutableSet<AppSettingsWindowController *> *copiedWindowControllers = [self.appSettingsControllers mutableCopy];

    for (AppSettingsWindowController *windowController in self.appSettingsControllers) {
        if ([windowController.lifetimeId raw_value] == lifetime_id) {
            [windowController close];
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.appSettingsControllers = copiedWindowControllers;
}

@end
