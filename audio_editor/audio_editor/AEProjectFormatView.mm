//
//  AEProjectFormatView.mm
//

#import "AEProjectFormatView.h"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_format_setup.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_stl_utils.h>
#include <cpp_utils/yas_unowned.h>

using namespace yas;
using namespace yas::ae;

@interface AEProjectFormatView ()
@property (nonatomic, weak) IBOutlet NSTextField *sampleRateTitleLabel;
@property (nonatomic, weak) IBOutlet NSTextField *channelTitleLabel;
@property (nonatomic, weak) IBOutlet NSPopUpButton *sampleRatePopUp;
@property (nonatomic, weak) IBOutlet NSTextField *channelValueLabel;
@property (nonatomic, weak) IBOutlet NSSlider *channelSlider;
@property (nonatomic, weak) IBOutlet NSStepper *channelStepper;
@end

@implementation AEProjectFormatView {
    std::weak_ptr<project_format_setup> _setup;
    observing::canceller_pool _pool;
}

+ (instancetype)instantiate {
    NSNib *nib = [[NSNib alloc] initWithNibNamed:@"ProjectFormat" bundle:nil];

    NSArray *objects = nil;

    [nib instantiateWithOwner:nil topLevelObjects:&objects];

    for (AEProjectFormatView *object in objects) {
        if ([object isKindOfClass:[AEProjectFormatView class]]) {
            [object setup];
            return object;
        }
    }

    assertion_failure_if_not_test();

    return nil;
}

- (void)setup {
    auto const &setup = hierarchy::project_setup_dialog_lifetime()->format_setup;
    self->_setup = setup;

    [self.sampleRatePopUp removeAllItems];

    NSMutableArray<NSString *> *titles = [[NSMutableArray alloc] initWithCapacity:setup->available_sample_rates.size()];
    for (auto const &sample_rate : setup->available_sample_rates) {
        [titles addObject:[NSString stringWithFormat:@"%@Hz", @(sample_rate)]];
    }

    [self.sampleRatePopUp addItemsWithTitles:titles];

    auto *unowned_self = make_unowned(self);

    self.channelSlider.minValue = setup->min_channel_count;
    self.channelSlider.maxValue = setup->max_channel_count;
    self.channelStepper.minValue = setup->min_channel_count;
    self.channelStepper.maxValue = setup->max_channel_count;

    setup
        ->observe_sample_rate_index(
            [unowned_self](std::size_t const &idx) { [unowned_self.object.sampleRatePopUp selectItemAtIndex:idx]; })
        .sync()
        ->add_to(self->_pool);

    setup
        ->observe_channel_count([unowned_self](uint32_t const &count) {
            auto *view = unowned_self.object;
            view.channelValueLabel.stringValue = [NSString stringWithFormat:@"%@", @(count)];
            view.channelStepper.intValue = count;
            view.channelSlider.intValue = count;
        })
        .sync()
        ->add_to(self->_pool);
}

- (IBAction)sampleRatePopUpDidSelect:(NSPopUpButton *)popUp {
    if (auto const setup = self->_setup.lock()) {
        setup->set_sample_rate_index(popUp.indexOfSelectedItem);
    }
}

- (IBAction)channelSliderDidChange:(NSSlider *)sender {
    if (auto const setup = self->_setup.lock()) {
        setup->set_channel_count(sender.intValue);
    }
}

- (IBAction)channelStepperDidChange:(NSStepper *)sender {
    if (auto const setup = self->_setup.lock()) {
        setup->set_channel_count(sender.intValue);
    }
}

@end
