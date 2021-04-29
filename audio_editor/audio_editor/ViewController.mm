//
//  ViewController.m
//

#import "ViewController.h"
#include <audio_editor_core/audio_editor_core_umbrella.h>
#include <cpp_utils/yas_cf_utils.h>
#import <objc_utils/yas_objc_unowned.h>

using namespace yas;
using namespace yas::ae;

@interface ViewController ()

@property (nonatomic, weak) IBOutlet NSTextField *statusLabel;

@end

@implementation ViewController {
    std::shared_ptr<project_view_presenter> _presenter;
    observing::canceller_pool _pool;
}

- (void)setupWithProjectID:(std::string const &)project_id {
    self->_presenter = project_view_presenter::make_shared(project_id);

    auto unowned = [[YASUnownedObject<ViewController *> alloc] initWithObject:self];

    self->_presenter
        ->observe_state_string([unowned](std::string const &string) {
            unowned.object.statusLabel.stringValue = (__bridge NSString *)to_cf_object(string);
        })
        .sync()
        ->add_to(self->_pool);
}

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
}

@end
