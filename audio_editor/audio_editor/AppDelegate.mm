//
//  AppDelegate.mm
//

#import "AppDelegate.h"
#import <audio_editor_core/AppWindowPresenter.h>
#include <audio_editor_core/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

@interface AppDelegate ()

@property (nonatomic) AppWindowPresenter *windowPresenter;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    if (NSClassFromString(@"XCTestCase") != nil) {
        return;
    }

    hierarchy::app_lifecycle()->add();

    self.windowPresenter = [[AppWindowPresenter alloc] init];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (BOOL)respondsToSelector:(SEL)aSelector {
    if (auto const action_kind = [self actionKindForSelector:aSelector]) {
        return [self->_windowPresenter respondsToAction:action_kind.value()];
    } else {
        return [super respondsToSelector:aSelector];
    }
}

- (IBAction)openDocument:(id)sender {
    [self->_windowPresenter performAction:app_window_action_kind::open_project];
}

- (IBAction)openAppSettings:(id)sender {
    [self->_windowPresenter performAction:app_window_action_kind::open_app_settings];
}

#pragma mark -

- (std::optional<app_window_action_kind>)actionKindForSelector:(SEL)selector {
    if (selector == @selector(openDocument:)) {
        return app_window_action_kind::open_project;
    } else if (selector == @selector(openAppSettings:)) {
        return app_window_action_kind::open_app_settings;
    } else {
        return std::nullopt;
    }
}

@end
