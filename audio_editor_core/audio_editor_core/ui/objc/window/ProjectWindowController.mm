//
//  ProjectWindowController.mm
//

#import "ProjectWindowController.h"
#import <audio_editor_core/AEMetalViewController.h>
#include <audio_editor_core/ae_project_presenter.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_cf_utils.h>
#include <cpp_utils/yas_unowned.h>
#include <cpp_utils/yas_url.h>

using namespace yas;
using namespace yas::ae;

@interface ProjectWindowController ()

@end

@implementation ProjectWindowController {
    std::shared_ptr<project_presenter> _presenter;
    observing::canceller_pool _pool;
}

+ (instancetype)instantiateWithLifetimeID:(yas::ae::project_lifetime_id const &)lifetime_id {
    NSStoryboard *storyboard = [NSStoryboard storyboardWithName:@"Window"
                                                         bundle:[NSBundle bundleForClass:[self class]]];
    ProjectWindowController *windowController = [storyboard instantiateInitialController];

    NSAssert([windowController isKindOfClass:[ProjectWindowController class]], @"");

    [windowController setupWithLifetimeID:lifetime_id];

    return windowController;
}

- (void)setupWithLifetimeID:(project_lifetime_id const &)lifetime_id {
    self->_presenter = project_presenter::make_shared(lifetime_id);
    self.window.title = (__bridge NSString *)to_cf_object(self->_presenter->title());

    if (auto const url = self->_presenter->represented_url(); url.has_value()) {
        self.window.representedURL = (__bridge NSURL *)url.value().cf_url();
    }

    if (auto const auto_save_name = self->_presenter->auto_save_name(); auto_save_name.has_value()) {
        self.window.frameAutosaveName = (__bridge NSString *)to_cf_object(auto_save_name.value());
    }

    auto unowned = make_unowned(self);

    self->_presenter
        ->observe([unowned, lifetime_id](project_presenter_event const &event) {
            AEMetalViewController *content = (AEMetalViewController *)unowned.object.contentViewController;

            if (![content isKindOfClass:[AEMetalViewController class]]) {
                assertion_failure_if_not_test();
                return;
            }

            [content setupWithProjectLifetimeID:lifetime_id];
        })
        .sync()
        ->add_to(self->_pool);
}

- (project_lifetime_id const &)lifetime_id {
    return self->_presenter->lifetime_id;
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    return self->_presenter->should_close();
}

@end
