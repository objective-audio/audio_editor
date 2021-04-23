//
//  AEWindowController.mm
//

#import "AEWindowController.h"
#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_window_presenter.h>
#include <cpp_utils/yas_cf_utils.h>

using namespace yas;
using namespace yas::ae;

@interface AEWindowController ()

@end

@implementation AEWindowController {
    std::shared_ptr<window_presenter> _presenter;
}

- (void)setupWithProjectID:(uintptr_t const &)project_id {
    self->_presenter = window_presenter::make_shared(project_id);
    self.window.title = (__bridge NSString *)to_cf_object(self->_presenter->title());
}

- (void)windowDidLoad {
    [super windowDidLoad];
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    if (auto const project = self->_project.lock()) {
        bool const can_close = project->can_close();
        project->request_close();
        return can_close;
    }
    return YES;
}

@end
