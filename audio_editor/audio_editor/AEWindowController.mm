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
    project_ptr _project;
}

- (void)setupWithProject:(project_ptr const &)project {
    self->_project = project;

    self.window.title = (__bridge NSString *)to_cf_object(self->_project->file_url().last_path_component());
}

- (void)windowDidLoad {
    [super windowDidLoad];
}

@end
