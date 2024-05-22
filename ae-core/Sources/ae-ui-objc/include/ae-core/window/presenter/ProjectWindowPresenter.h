#import <Foundation/Foundation.h>
#import <ae-core/global/value_types/ProjectLifetimeId.h>

NS_ASSUME_NONNULL_BEGIN

@interface ProjectWindowPresenter : NSObject

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithLifetimeId:(ProjectLifetimeId *)lifetimeId;

@property (nonatomic, readonly) ProjectLifetimeId *lifetimeId;
@property (nonatomic, readonly) NSString *title;
@property (nonatomic, readonly, nullable) NSString *autoSaveName;
@property (nonatomic, readonly, nullable) NSURL *representedUrl;
@property (nonatomic, readonly) BOOL shouldClose;

- (void)observe:(void (^)(void))handler;

@end

NS_ASSUME_NONNULL_END
