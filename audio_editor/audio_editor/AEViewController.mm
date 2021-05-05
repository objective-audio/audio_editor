//
//  AEViewController.m
//

#import "AEViewController.h"
#include <audio_editor_core/audio_editor_core_umbrella.h>
#include <cpp_utils/yas_cf_utils.h>
#import <objc_utils/yas_objc_unowned.h>

using namespace yas;
using namespace yas::ae;

@interface AEViewController ()

@property (nonatomic, weak) IBOutlet NSTextField *statusLabel;
@property (nonatomic, weak) IBOutlet NSTextField *fileInfoLabel;
@property (nonatomic, weak) IBOutlet NSTextField *playerLabel;
@property (nonatomic, weak) IBOutlet NSButton *isPlayButton;

@property (nonatomic) NSTimer *timer;

@end

@implementation AEViewController {
    std::shared_ptr<project_view_presenter> _presenter;
    observing::canceller_pool _pool;
}

- (void)dealloc {
    [self.timer invalidate];
}

- (void)setupWithProjectID:(std::string const &)project_id {
    self->_presenter = project_view_presenter::make_shared(project_id);

    auto unowned = [[YASUnownedObject<AEViewController *> alloc] initWithObject:self];

    self->_presenter
        ->observe_state_string([unowned](std::string const &string) {
            unowned.object.statusLabel.stringValue = (__bridge NSString *)to_cf_object(string);
        })
        .sync()
        ->add_to(self->_pool);

    self->_presenter
        ->observe_file_info_string([unowned](std::string const &string) {
            unowned.object.fileInfoLabel.stringValue = (__bridge NSString *)to_cf_object(string);
        })
        .sync()
        ->add_to(self->_pool);

    self->_presenter
        ->observe_play_button_string([unowned](std::string const &string) {
            unowned.object.isPlayButton.title = (__bridge NSString *)to_cf_object(string);
        })
        .sync()
        ->add_to(self->_pool);

    self.timer = [NSTimer
        scheduledTimerWithTimeInterval:1.0 / 10.0
                               repeats:YES
                                 block:^(NSTimer *timer) {
                                     AEViewController *viewController = unowned.object;
                                     viewController.playerLabel.stringValue =
                                         (__bridge NSString *)to_cf_object(viewController->_presenter->player_string());
                                 }];
}

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
}

- (IBAction)playButtonClicked:(NSButton *)sender {
    self->_presenter->play_button_clicked();
}

@end
