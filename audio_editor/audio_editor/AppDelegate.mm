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
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_setup_presenter.h>
#include <cpp_utils/yas_cf_utils.h>
#include <cpp_utils/yas_unowned.h>

using namespace yas;
using namespace yas::ae;

@interface AppDelegate ()

@property (nonatomic) NSMutableSet<AEWindowController *> *windowControllers;

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

    auto *const unowned = make_unowned(self);

    self->_presenter
        ->observe_window([unowned](auto const &event) {
            switch (event.type) {
                case app_presenter_window_event_type::make_and_show_window_controller: {
                    [unowned.object makeAndShowWindowControllerWithLifetimeID:event.lifetime_id];
                } break;
                case app_presenter_window_event_type::dispose_window_controller: {
                    [unowned.object disposeWindowControllerWithLifetimeID:event.lifetime_id];
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
    } else {
        return [super respondsToSelector:aSelector];
    }
}

- (void)openDocument:(id)sender {
    self->_presenter->open_project_setup_dialog();
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
            url const file_url{to_string((__bridge CFStringRef)unowned_panel.object.URL.absoluteString)};
            presenter->select_directory(file_url);
        }

        presenter->did_close_dialog();
    }];
}

- (void)makeAndShowWindowControllerWithLifetimeID:(window_lifetime_id const &)lifetime_id {
    NSStoryboard *storyboard = [NSStoryboard storyboardWithName:@"Window" bundle:nil];
    AEWindowController *windowController = [storyboard instantiateInitialController];
    NSAssert([windowController isKindOfClass:[AEWindowController class]], @"");
    [windowController setupWithLifetimeID:lifetime_id];
    [self.windowControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)disposeWindowControllerWithLifetimeID:(window_lifetime_id const &)lifetime_id {
    NSMutableSet<AEWindowController *> *copiedWindowControllers = [self.windowControllers mutableCopy];

    for (AEWindowController *windowController in self.windowControllers) {
        if (windowController.lifetime_id == lifetime_id) {
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.windowControllers = copiedWindowControllers;
}

@end
