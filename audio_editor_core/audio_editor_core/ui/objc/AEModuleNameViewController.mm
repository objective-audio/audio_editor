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
};
}

@interface AEModuleNameViewController ()

@property (nonatomic, weak) IBOutlet NSTextField *titleLabel;
@property (nonatomic, weak) IBOutlet NSTextField *nameTextField;

@end

@implementation AEModuleNameViewController {
    module_name_vc_cpp _cpp;
}

+ (instancetype)instantiateWithSheetLifetimeId:(sheet_lifetime_id const &)sheet_lifetime_id {
    auto *const storyboard = [NSStoryboard storyboardWithName:@"ModuleName"
                                                       bundle:[NSBundle bundleForClass:[self class]]];
    AEModuleNameViewController *vc = [storyboard instantiateInitialController];
    vc->_cpp.presenter = module_name_presenter::make_shared(sheet_lifetime_id);
    vc.preferredContentSize = NSMakeSize(400.0, 150.0);
    return vc;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    self.nameTextField.stringValue = (__bridge NSString *)to_cf_object(self->_cpp.presenter->name());
}

- (IBAction)done:(NSButton *)sender {
    self->_cpp.presenter->done(to_string((__bridge CFStringRef)self.nameTextField.stringValue));
}

- (IBAction)cancel:(NSButton *)sender {
    self->_cpp.presenter->cancel();
}

@end
