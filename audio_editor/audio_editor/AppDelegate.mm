//
//  AppDelegate.mm
//

#import "AppDelegate.h"
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#import "ae_app_global.h"

using namespace yas;
using namespace yas::ae;

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
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
        NSStoryboard *storyboard = [NSStoryboard storyboardWithName:@"Window" bundle:nil];
        NSWindowController *windowController = [storyboard instantiateInitialController];
        [windowController showWindow:self];

        windowController.window.title = panel.URL.lastPathComponent;
    }
}

@end
