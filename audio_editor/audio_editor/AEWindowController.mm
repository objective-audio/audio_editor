//
//  AEWindowController.mm
//

#import "AEWindowController.h"
#include <audio_editor_core/audio_editor_core_umbrella.h>
#include <cpp_utils/yas_cf_utils.h>
#import "AEMetalViewController.h"

using namespace yas;
using namespace yas::ae;

@interface AEWindowController ()

@end

@implementation AEWindowController {
    std::shared_ptr<window_presenter> _presenter;
}

- (void)setupWithProjectID:(std::string const &)project_id {
    self->_presenter = window_presenter::make_shared(project_id);
    self.window.title = (__bridge NSString *)to_cf_object(self->_presenter->title());

    AEMetalViewController *content = (AEMetalViewController *)self.contentViewController;
    NSAssert([content isKindOfClass:[AEMetalViewController class]], @"");
    [content setupWithProjectID:project_id];
}

- (std::string const &)project_id {
    return self->_presenter->project_id();
}

- (void)windowDidLoad {
    [super windowDidLoad];
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    return self->_presenter->should_close();
}

@end
