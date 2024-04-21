#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ProjectFormatPresenterDelegate <NSObject>

- (void)sampleRateIndexDidChange:(NSInteger)index;
- (void)channelCountDidChange:(uint32_t)count;

@end

@interface ProjectFormatPresenter : NSObject

- (instancetype)initWithDelegate:(id<ProjectFormatPresenterDelegate>)delegate;
- (instancetype)init NS_UNAVAILABLE;

- (void)setupObserving;

@property (nonatomic, readonly) NSInteger availableSampleRatesCount;
- (uint32_t)availableSampleRateAtIndex:(NSInteger)index;
@property (nonatomic, readonly) uint32_t minChannelCount;
@property (nonatomic, readonly) uint32_t maxChannelCount;

- (void)setSampleRateIndex:(NSInteger)index;
- (void)setChannelCount:(uint32_t)count;

@end

NS_ASSUME_NONNULL_END
