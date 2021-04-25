//
//  AppDelegate.mm
//

#import "AppDelegate.h"
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#import <audio_editor_core/ae_app.h>
#import <audio_editor_core/ae_app_presenter.h>
#import <cpp_utils/yas_cf_utils.h>
#import <objc_utils/yas_objc_unowned.h>
#import "AEWindowController.h"

using namespace yas;
using namespace yas::ae;

@interface AppDelegate ()

@property (nonatomic) NSMutableSet<AEWindowController *> *windowControllers;

@end

@implementation AppDelegate {
    app_presenter _presenter;
    observing::canceller_pool _pool;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    self.windowControllers = [[NSMutableSet alloc] init];

    auto unowned = [[YASUnownedObject<AppDelegate *> alloc] initWithObject:self];

    self->_presenter
        .observe_event([unowned](auto const &event) {
            switch (event.type) {
                case app_presenter_event_type::open_file_dialog: {
                    [unowned.object openFileDialog];
                } break;
                case app_presenter_event_type::make_and_show_window_controller: {
                    [unowned.object makeAndShowWindowControllerWithProjectID:event.project_id];
                } break;
                case app_presenter_event_type::dispose_window_controller: {
                    [unowned.object disposeWindowControllerWithProjectID:event.project_id];
                } break;
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
        return self->_presenter.can_open_file_dialog();
    } else {
        return [super respondsToSelector:aSelector];
    }
}

- (void)openDocument:(id)sender {
    self->_presenter.open_file_dialog();
}

#pragma mark - private

- (void)openFileDialog {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    panel.allowsMultipleSelection = NO;
    panel.canChooseFiles = YES;
    panel.canChooseDirectories = NO;
    panel.allowedContentTypes = @[UTTypeAudio];

    if ([panel runModal] == NSModalResponseOK) {
        url const file_url{to_string((__bridge CFStringRef)panel.URL.absoluteString)};
        self->_presenter.select_file(file_url);
    }
}

- (void)makeAndShowWindowControllerWithProjectID:(std::string const &)project_id {
    NSStoryboard *storyboard = [NSStoryboard storyboardWithName:@"Window" bundle:nil];
    AEWindowController *windowController = [storyboard instantiateInitialController];
    NSAssert([windowController isKindOfClass:[AEWindowController class]], @"");
    [windowController setupWithProjectID:project_id];
    [self.windowControllers addObject:windowController];
    [windowController showWindow:nil];
}

- (void)disposeWindowControllerWithProjectID:(std::string const &)project_id {
    NSMutableSet<AEWindowController *> *copiedWindowControllers = [self.windowControllers mutableCopy];

    for (AEWindowController *windowController in self.windowControllers) {
        if (windowController.project_id == project_id) {
            [copiedWindowControllers removeObject:windowController];
        }
    }

    self.windowControllers = copiedWindowControllers;
}

@end
