//
//  AppDelegate.mm
//

#import "AppDelegate.h"
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#include <audio_editor_core/ae_app_dialog_sub_level.h>
#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_app_presenter.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_setup_presenter.h>
#include <cpp_utils/yas_cf_utils.h>
#include <cpp_utils/yas_unowned.h>
#import "AEProjectFormatView.h"
#import "AEWindowController.h"

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

    hierarchy::app_level_router()->add_level();
    self->_presenter = app_presenter::make_shared();

    self.windowControllers = [[NSMutableSet alloc] init];

    auto *const unowned = make_unowned(self);

    self->_presenter
        ->observe_window([unowned](auto const &event) {
            switch (event.type) {
                case app_presenter_window_event_type::make_and_show_window_controller: {
                    [unowned.object makeAndShowWindowControllerWithProjectID:event.project_id];
                } break;
                case app_presenter_window_event_type::dispose_window_controller: {
                    [unowned.object disposeWindowControllerWithProjectID:event.project_id];
                } break;
            }
        })
        .sync()
        ->add_to(self->_pool);

    self->_presenter
        ->observe_dialog([unowned](std::optional<app_dialog_sub_level> const &sub_level) {
            if (!sub_level.has_value()) {
                // do nothing.
            } else if (auto const level = get_level<app_dialog_level>(sub_level)) {
                [unowned.object openAudioFileDialog];
            } else if (auto const level = get_level<project_setup_dialog_level>(sub_level)) {
                [unowned.object openProjectSetupDialog];
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

- (void)openAudioFileDialog {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    panel.allowsMultipleSelection = NO;
    panel.canChooseFiles = YES;
    panel.canChooseDirectories = NO;
    panel.allowedContentTypes = @[UTTypeAudio];

    auto const unowned_self = make_unowned(self);
    auto const unowned_panel = make_unowned(panel);

    [panel beginWithCompletionHandler:[unowned_self, unowned_panel](NSModalResponse result) {
        if (result == NSModalResponseOK) {
            url const file_url{to_string((__bridge CFStringRef)unowned_panel.object.URL.absoluteString)};
            unowned_self.object->_presenter->select_audio_file(file_url);
        }

        unowned_self.object->_presenter->did_close_dialog();
    }];
}

- (void)openProjectSetupDialog {
    auto const presenter = project_setup_presenter::make_shared();

    NSOpenPanel *panel = [NSOpenPanel openPanel];
    panel.allowsMultipleSelection = NO;
    panel.canChooseFiles = YES;       // NO
    panel.canChooseDirectories = NO;  // YES
    panel.canCreateDirectories = NO;  // YES
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

- (void)makeAndShowWindowControllerWithProjectID:(project_id const &)project_id {
    NSStoryboard *storyboard = [NSStoryboard storyboardWithName:@"Window" bundle:nil];
    AEWindowController *windowController = [storyboard instantiateInitialController];
    NSAssert([windowController isKindOfClass:[AEWindowController class]], @"");
    [windowController setupWithProjectID:project_id];
    [self.windowControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)disposeWindowControllerWithProjectID:(project_id const &)project_id {
    NSMutableSet<AEWindowController *> *copiedWindowControllers = [self.windowControllers mutableCopy];

    for (AEWindowController *windowController in self.windowControllers) {
        if (windowController.project_id == project_id) {
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.windowControllers = copiedWindowControllers;
}

@end
