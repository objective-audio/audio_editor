//
//  AEModuleNameViewController.mm
//

#import "AEModuleNameViewController.h"
#include <audio_editor_core/ae_module_name_presenter.h>
#include <cpp_utils/yas_cf_utils.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct module_name_vc_cpp {
    std::shared_ptr<module_name_presenter> presenter;

    observing::notifier_ptr<module_name_vc_event> const event_notifier =
        observing::notifier<module_name_vc_event>::make_shared();
};
}

@interface AEModuleNameViewController ()

@property (nonatomic, weak) IBOutlet NSTextField *titleLabel;
@property (nonatomic, weak) IBOutlet NSTextField *nameTextField;

@end

@implementation AEModuleNameViewController {
    module_name_vc_cpp _cpp;
}

+ (instancetype)instantiateWithProjectId:(std::string const &)project_id moduleRange:(time::range const)module_range {
    auto *const storyboard = [NSStoryboard storyboardWithName:@"ModuleName" bundle:nil];
    AEModuleNameViewController *vc = [storyboard instantiateInitialController];
    vc->_cpp.presenter = module_name_presenter::make_shared(project_id, module_range);
    vc.preferredContentSize = NSMakeSize(400.0, 150.0);
    return vc;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    self.nameTextField.stringValue = (__bridge NSString *)to_cf_object(self->_cpp.presenter->name());
}

- (IBAction)done:(NSButton *)sender {
    self->_cpp.presenter->set_name(to_string((__bridge CFStringRef)self.nameTextField.stringValue));
    self->_cpp.event_notifier->notify(module_name_vc_event::done);
}

- (IBAction)cancel:(NSButton *)sender {
    self->_cpp.event_notifier->notify(module_name_vc_event::canceled);
}

- (observing::endable)observe_event:(std::function<void(module_name_vc_event const &)> &&)handler {
    return self->_cpp.event_notifier->observe(std::move(handler));
}

@end
