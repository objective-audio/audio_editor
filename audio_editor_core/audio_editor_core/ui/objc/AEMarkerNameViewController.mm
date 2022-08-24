//
//  AEMarkerNameViewController.m
//  audio_editor_core
//
//  Created by Yuki Yasoshima on 2022/08/16.
//

#import "AEMarkerNameViewController.h"
#include <cpp_utils/yas_cf_utils.h>
#include <audio_editor_core/ae_marker_name_presenter.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct marker_name_vc_cpp {
    std::shared_ptr<marker_name_presenter> presenter;
};
}

@interface AEMarkerNameViewController ()

@property (nonatomic, weak) IBOutlet NSTextField *titleLabel;
@property (nonatomic, weak) IBOutlet NSTextField *nameTextField;

@end

@implementation AEMarkerNameViewController {
    marker_name_vc_cpp _cpp;
}

+ (instancetype)instantiateWithSheetLifetimeId:(yas::ae::sheet_lifetime_id const &)sheet_lifetime_id {
    auto *const storyboard = [NSStoryboard storyboardWithName:@"MarkerName"
                                                       bundle:[NSBundle bundleForClass:[self class]]];
    AEMarkerNameViewController *vc = [storyboard instantiateInitialController];
    vc->_cpp.presenter = marker_name_presenter::make_shared(sheet_lifetime_id);
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
