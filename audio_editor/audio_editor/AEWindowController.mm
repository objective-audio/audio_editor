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
    return self->_presenter->project_id;
}

- (void)windowDidLoad {
    [super windowDidLoad];

    auto const visible_frame = self.window.screen.visibleFrame;
    auto const height = std::min(CGRectGetHeight(visible_frame), 300.0);
    auto const frame = CGRectMake(CGRectGetMinX(visible_frame), CGRectGetMaxY(visible_frame) - height,
                                  CGRectGetWidth(visible_frame), height);
    [self.window setFrame:frame display:NO];
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    return self->_presenter->should_close();
}

@end
