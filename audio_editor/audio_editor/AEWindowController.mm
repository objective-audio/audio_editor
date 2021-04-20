//
//  AEWindowController.mm
//

#import "AEWindowController.h"
#include <cpp_utils/yas_cf_utils.h>
#include "ae_app_global.h"

using namespace yas;
using namespace yas::ae;

@interface AEWindowController ()

@end

@implementation AEWindowController {
    app_project_interface_wptr _project;
}

- (void)setupWithProjectID:(uintptr_t const &)project_id {
    self->_project_id = project_id;

    if (auto const project = app_global()->project_for_id(project_id)) {
        self->_project = project;
        self.window.title = (__bridge NSString *)to_cf_object(project->file_url().last_path_component());
    }
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
