//
//  AEMetalViewController.mm
//

#import "AEMetalViewController.h"
#include <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface AEMetalViewController ()

@end

@implementation AEMetalViewController {
    std::shared_ptr<ui_root> _ui_root;
}

- (void)setupWithProjectID:(std::string const &)project_id {
    auto const renderer = ui::renderer::make_shared(
        ui::metal_system::make_shared(objc_ptr_with_move_object(MTLCreateSystemDefaultDevice()).object()));
    [self setRenderer:renderer];
    self->_ui_root = ui_root::make_shared(renderer, project_view_presenter::make_shared(project_id));
}

@end
