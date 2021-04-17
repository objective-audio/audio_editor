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

@end

@implementation AppDelegate {
    observing::canceller_pool _pool;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    auto unowned = [[YASUnownedObject<AppDelegate *> alloc] initWithObject:self];

    ae::app_global
        ->observe_projects([unowned](app::projects_t::event const &event) {
            switch (event.type) {
                case observing::vector::event_type::any: {
                    for (auto const &project : event.elements) {
                        [unowned.object showWindowWithProject:project];
                    }
                } break;
                case observing::vector::event_type::inserted: {
                    [unowned.object showWindowWithProject:*event.element];
                } break;
                case observing::vector::event_type::replaced:
                    break;
                case observing::vector::event_type::erased: {
                    [unowned.object hideWindowWithProject:*event.element];
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

- (void)showWindowWithProject:(ae::project_ptr const &)project {
    NSStoryboard *storyboard = [NSStoryboard storyboardWithName:@"Window" bundle:nil];
    AEWindowController *windowController = [storyboard instantiateInitialController];
    NSAssert([windowController isKindOfClass:[AEWindowController class]], @"");
    [windowController setupWithProject:project];
    [windowController showWindow:nil];
}

- (void)hideWindowWithProject:(ae::project_ptr const &)project {
    [NSApplication.sharedApplication
        enumerateWindowsWithOptions:kNilOptions
                         usingBlock:^(NSWindow *_Nonnull window, BOOL *_Nonnull stop) {
                             AEWindowController *windowController = window.windowController;
                             if ([windowController isKindOfClass:[AEWindowController class]]) {
                                 if (windowController.project == project) {
                                     [windowController dismissController:nil];
                                 }
                             }
                         }];
}

@end
