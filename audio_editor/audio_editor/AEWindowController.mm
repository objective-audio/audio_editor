//
//  AEWindowController.mm
//

#import "AEWindowController.h"
#include <audio_editor_core/ae_window_presenter.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_cf_utils.h>
#include <cpp_utils/yas_unowned.h>
#import "AEMetalViewController.h"

using namespace yas;
using namespace yas::ae;

@interface AEWindowController ()

@end

@implementation AEWindowController {
    std::shared_ptr<window_presenter> _presenter;
    observing::canceller_pool _pool;
}

- (void)setupWithLifetimeID:(window_lifetime_id const &)lifetime_id {
    self->_presenter = window_presenter::make_shared(lifetime_id);
    self.window.title = (__bridge NSString *)to_cf_object(self->_presenter->title());

    auto unowned = make_unowned(self);

    self->_presenter
        ->observe([unowned, lifetime_id](const window_presenter_event &event) {
            AEMetalViewController *content = (AEMetalViewController *)unowned.object.contentViewController;

            if (![content isKindOfClass:[AEMetalViewController class]]) {
                assertion_failure_if_not_test();
                return;
            }

            [content setupWithWindowLifetimeID:lifetime_id];
        })
        .sync()
        ->add_to(self->_pool);
}

- (window_lifetime_id const &)lifetime_id {
    return self->_presenter->lifetime_id;
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
