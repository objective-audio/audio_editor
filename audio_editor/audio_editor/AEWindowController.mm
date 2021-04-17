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
    project_wptr _project;
}

- (void)setupWithProject:(project_ptr const &)project {
    self->_project = project;

    self.window.title = (__bridge NSString *)to_cf_object(project->file_url().last_path_component());
}

- (project_ptr)project {
    return self->_project.lock();
}

- (void)windowDidLoad {
    [super windowDidLoad];
}

@end
