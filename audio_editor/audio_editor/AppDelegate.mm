//
//  AppDelegate.mm
//

#import "AppDelegate.h"
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#import <audio_editor_core/AEProjectFormatView.h>
#import <audio_editor_core/AEWindowController.h>
#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_app_modal_sub_lifetime.h>
#include <audio_editor_core/ae_app_presenter.h>
#import <audio_editor_core/ae_app_settings_lifetime_id+objc.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_setup_presenter.h>
#import <audio_editor_core/ae_window_lifetime_id+objc.h>
#import <audio_editor_core/audio_editor_core-Swift.h>
#include <cpp_utils/yas_cf_utils.h>
#include <cpp_utils/yas_unowned.h>

using namespace yas;
using namespace yas::ae;

@interface AppDelegate ()

@property (nonatomic) NSMutableSet<AEWindowController *> *windowControllers;
@property (nonatomic) NSMutableSet<ProjectSettingsWindowController *> *projectSettingsWindowControllers;
@property (nonatomic) NSMutableSet<AppSettingsWindowController *> *appSettingsWindowControllers;

@end

@implementation AppDelegate {
    std::shared_ptr<app_presenter> _presenter;
    observing::canceller_pool _pool;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    if (NSClassFromString(@"XCTestCase") != nil) {
        return;
    }

    hierarchy::app_lifecycle()->add();
    self->_presenter = app_presenter::make_shared();

    self.windowControllers = [[NSMutableSet alloc] init];
    self.projectSettingsWindowControllers = [[NSMutableSet alloc] init];
    self.appSettingsWindowControllers = [[NSMutableSet alloc] init];

    auto *const unowned = make_unowned(self);

    self->_presenter
        ->observe_window([unowned](auto const &event) {
            switch (event.type) {
                case app_presenter_window_event_type::make_and_show_window_controller: {
                    [unowned.object makeAndShowWindowControllerWithLifetimeID:event.window_lifetime_id()];
                } break;
                case app_presenter_window_event_type::dispose_window_controller: {
                    [unowned.object disposeWindowControllerWithLifetimeID:event.window_lifetime_id()];
                } break;
                case app_presenter_window_event_type::make_and_show_project_settings: {
                    [unowned.object makeAndShowProjectSettingsWithLifetimeID:event.window_lifetime_id()];
                } break;
                case app_presenter_window_event_type::dispose_project_settings: {
                    [unowned.object disposeProjectSettingsWithLifetimeID:event.window_lifetime_id()];
                } break;
                case app_presenter_window_event_type::make_and_show_app_settings: {
                    [unowned.object makeAndShowAppSettingsWithLifetimeID:event.app_settings_lifetime_id()];
                } break;
                case app_presenter_window_event_type::dispose_app_settings: {
                    [unowned.object disposeAppSettingsWithLifetimeID:event.app_settings_lifetime_id()];
                } break;
            }
        })
        .sync()
        ->add_to(self->_pool);

    self->_presenter
        ->observe_dialog([unowned](std::optional<app_modal_sub_lifetime> const &sub_lifetime) {
            using kind = app_modal_sub_lifetime_kind;

            switch (to_kind(sub_lifetime)) {
                case kind::none:
                    // panelは強制的に閉じれないので何もしない
                    break;

                case kind::project_setup_dialog:
                    [unowned.object openProjectSetupDialog];
                    break;
            }
        })
        .sync()
        ->add_to(self->_pool);
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (BOOL)respondsToSelector:(SEL)aSelector {
    if (aSelector == @selector(openDocument:)) {
        return self->_presenter->can_open_dialog();
    } else if (aSelector == @selector(openAppSettings:)) {
        return self->_presenter->can_open_app_settings();
    } else {
        return [super respondsToSelector:aSelector];
    }
}

- (IBAction)openDocument:(id)sender {
    self->_presenter->open_project_setup_dialog();
}

- (IBAction)openAppSettings:(id)sender {
    self->_presenter->open_app_settings();
}

#pragma mark - private

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

- (void)makeAndShowWindowControllerWithLifetimeID:(window_lifetime_id const &)lifetime_id {
    AEWindowController *windowController = [AEWindowController instantiateWithLifetimeID:lifetime_id];
    [self.windowControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)disposeWindowControllerWithLifetimeID:(window_lifetime_id const &)lifetime_id {
    NSMutableSet<AEWindowController *> *copiedWindowControllers = [self.windowControllers mutableCopy];

    for (AEWindowController *windowController in self.windowControllers) {
        if (windowController.lifetime_id == lifetime_id) {
            [windowController close];
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.windowControllers = copiedWindowControllers;
}

- (void)makeAndShowProjectSettingsWithLifetimeID:(window_lifetime_id const &)lifetime_id {
    ProjectSettingsWindowController *windowController = [ProjectSettingsWindowController
        instantiateWithLifetimeId:[[WindowLifetimeId alloc] initWithRawValue:lifetime_id]];
    [self.projectSettingsWindowControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)disposeProjectSettingsWithLifetimeID:(window_lifetime_id const &)lifetime_id {
    NSMutableSet<ProjectSettingsWindowController *> *copiedWindowControllers =
        [self.projectSettingsWindowControllers mutableCopy];

    for (ProjectSettingsWindowController *windowController in self.projectSettingsWindowControllers) {
        if ([windowController.lifetimeId raw_value] == lifetime_id) {
            [windowController close];
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.projectSettingsWindowControllers = copiedWindowControllers;
}

- (void)makeAndShowAppSettingsWithLifetimeID:(app_settings_lifetime_id const &)lifetime_id {
    AppSettingsWindowController *windowController = [AppSettingsWindowController
        instantiateWithLifetimeId:[[AppSettingsLifetimeId alloc] initWithRawValue:lifetime_id]];
    [self.appSettingsWindowControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)disposeAppSettingsWithLifetimeID:(app_settings_lifetime_id const &)lifetime_id {
    NSMutableSet<AppSettingsWindowController *> *copiedWindowControllers =
        [self.appSettingsWindowControllers mutableCopy];

    for (AppSettingsWindowController *windowController in self.appSettingsWindowControllers) {
        if ([windowController.lifetimeId raw_value] == lifetime_id) {
            [windowController close];
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.appSettingsWindowControllers = copiedWindowControllers;
}

@end
