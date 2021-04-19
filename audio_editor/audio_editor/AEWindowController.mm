//
//  AEWindowController.mm
//

#import "AEWindowController.h"
#include <cpp_utils/yas_cf_utils.h>

using namespace yas;
using namespace yas::ae;

@interface AEWindowController ()

@end

@implementation AEWindowController {
    app_project_interface_wptr _project;
}

- (void)setupWithProject:(app_project_interface_ptr const &)project {
    self->_project = project;

    self.window.title = (__bridge NSString *)to_cf_object(project->file_url().last_path_component());
}

- (app_project_interface_ptr)project {
    return self->_project.lock();
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
