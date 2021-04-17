//
//  AppDelegate.m
//

#import "AppDelegate.h"

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
    panel.allowedFileTypes = @[@"wav", @"aif", @"aiff", @"caf"];

    if ([panel runModal] == NSModalResponseOK) {
        NSStoryboard *storyboard = [NSStoryboard storyboardWithName:@"Window" bundle:nil];
        NSWindowController *windowController = [storyboard instantiateInitialController];
        [windowController showWindow:self];

        windowController.window.title = panel.URL.lastPathComponent;
    }
}

@end
