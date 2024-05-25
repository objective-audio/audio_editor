#import "ProjectFormatPresenter.h"
#include <ae-core/global/hierarchy.h>
#include <ae-core/project_format_dialog/features/project_format_setup.h>
#include <cpp-utils/assertion.h>
#include <cpp-utils/unowned.h>

using namespace yas;
using namespace yas::ae;

@interface ProjectFormatPresenter ()
@property (nonatomic, weak) id<ProjectFormatPresenterDelegate> delegate;
@end

@implementation ProjectFormatPresenter {
    std::weak_ptr<project_format_setup> _setup;
    observing::canceller_pool _pool;
}

- (instancetype)initWithDelegate:(id<ProjectFormatPresenterDelegate>)delegate {
    self = [super init];
    if (self) {
        self.delegate = delegate;
        self->_setup = hierarchy::project_setup_dialog_lifetime()->format_setup;
    }
    return self;
}

- (void)setupObserving {
    if (auto const setup = self->_setup.lock()) {
        auto *unowned_self = make_unowned(self);

        setup
            ->observe_sample_rate_index(
                [unowned_self](std::size_t const &idx) { [unowned_self.object.delegate sampleRateIndexDidChange:idx]; })
            .sync()
            ->add_to(self->_pool);

        setup
            ->observe_channel_count(
                [unowned_self](uint32_t const &count) { [unowned_self.object.delegate channelCountDidChange:count]; })
            .sync()
            ->add_to(self->_pool);
    } else {
        assertion_failure_if_not_test();
    }
}

- (NSInteger)availableSampleRatesCount {
    if (auto const setup = self->_setup.lock()) {
        return setup->available_sample_rates.size();
    } else {
        return 0;
    }
}

- (uint32_t)availableSampleRateAtIndex:(NSInteger)index {
    if (auto const setup = self->_setup.lock()) {
        return setup->available_sample_rates.at(index);
    } else {
        return 0;
    }
}

- (uint32_t)minChannelCount {
    if (auto const setup = self->_setup.lock()) {
        return setup->min_channel_count;
    } else {
        return 0;
    }
}

- (uint32_t)maxChannelCount {
    if (auto const setup = self->_setup.lock()) {
        return setup->max_channel_count;
    } else {
        return 0;
    }
}

- (void)setSampleRateIndex:(NSInteger)index {
    if (auto const setup = self->_setup.lock()) {
        setup->set_sample_rate_index(index);
    }
}

- (void)setChannelCount:(uint32_t)count {
    if (auto const setup = self->_setup.lock()) {
        setup->set_channel_count(count);
    }
}

@end
