//
//  AppDelegate.mm
//

#import "AppDelegate.h"
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#import <audio_editor_core/ae_app_global.h>
#include <cpp_utils/yas_cf_utils.h>
#import <objc_utils/yas_objc_unowned.h>
#import "AEWindowController.h"

using namespace yas;
using namespace yas::ae;

@interface AppDelegate ()

@property (nonatomic) NSMutableSet<AEWindowController *> *windowControllers;

@end

@implementation AppDelegate {
    app_delegate_presenter _presenter;
    observing::canceller_pool _pool;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    self.windowControllers = [[NSMutableSet alloc] init];

    auto unowned = [[YASUnownedObject<AppDelegate *> alloc] initWithObject:self];

    app_global()
        ->project_pool()
        ->observe_projects([unowned](auto const &event) {
            switch (event.type) {
                case project_pool_event_type::inserted: {
                    [unowned.object showWindowWithProjectID:event.project_id];
                } break;
                case project_pool_event_type::erased: {
                    [unowned.object hideWindowWithProjectID:event.project_id];
                } break;
            }
        })
        .sync()
        ->add_to(self->_pool);
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (void)openDocument:(id)sender {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    panel.allowsMultipleSelection = NO;
    panel.canChooseFiles = YES;
    panel.canChooseDirectories = NO;
    panel.allowedContentTypes = @[UTTypeAudio];

    if ([panel runModal] == NSModalResponseOK) {
        url const file_url{to_string((__bridge CFStringRef)panel.URL.absoluteString)};
        self->_presenter.add_project(file_url);
    }
}

- (void)showWindowWithProjectID:(uintptr_t const)project_id {
    NSStoryboard *storyboard = [NSStoryboard storyboardWithName:@"Window" bundle:nil];
    AEWindowController *windowController = [storyboard instantiateInitialController];
    NSAssert([windowController isKindOfClass:[AEWindowController class]], @"");
    [windowController setupWithProjectID:project_id];
    [self.windowControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)hideWindowWithProjectID:(uintptr_t const)project_id {
    NSMutableSet<AEWindowController *> *copiedWindowControllers = [self.windowControllers mutableCopy];

    for (AEWindowController *windowController in self.windowControllers) {
        if (windowController.project_id == project_id) {
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.windowControllers = copiedWindowControllers;
}

@end
