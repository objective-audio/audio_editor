//
//  AppDelegate.mm
//

#import "AppDelegate.h"
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#include <cpp_utils/yas_cf_utils.h>
#import <objc_utils/yas_objc_unowned.h>
#import "AEWindowController.h"
#import "ae_app_global.h"

using namespace yas;
using namespace yas::ae;

@interface AppDelegate ()

@property (nonatomic) NSMutableSet<AEWindowController *> *windowControllers;

@end

@implementation AppDelegate {
    observing::canceller_pool _pool;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    self.windowControllers = [[NSMutableSet alloc] init];

    auto unowned = [[YASUnownedObject<AppDelegate *> alloc] initWithObject:self];

    ae::app_global
        ->observe_project([unowned](auto const &event) {
            switch (event.type) {
                case app_projects_event_type::inserted: {
                    [unowned.object showWindowWithProject:event.project];
                } break;
                case app_projects_event_type::erased: {
                    [unowned.object hideWindowWithProject:event.project];
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
        ae::app_global->add_project(file_url);
    }
}

- (void)showWindowWithProject:(ae::app_project_interface_ptr const &)project {
    NSStoryboard *storyboard = [NSStoryboard storyboardWithName:@"Window" bundle:nil];
    AEWindowController *windowController = [storyboard instantiateInitialController];
    NSAssert([windowController isKindOfClass:[AEWindowController class]], @"");
    [windowController setupWithProject:project];
    [self.windowControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)hideWindowWithProject:(ae::app_project_interface_ptr const &)project {
    NSMutableSet<AEWindowController *> *copiedWindowControllers = [self.windowControllers mutableCopy];

    for (AEWindowController *windowController in self.windowControllers) {
        if (windowController.project == project) {
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.windowControllers = copiedWindowControllers;
}

@end
