//
//  SettingsWindowPresenter.h
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class WindowLifetimeId;

@interface SettingsWindowPresenter : NSObject

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithLifetimeId:(WindowLifetimeId *)lifetimeId;

@property (nonatomic, readonly) WindowLifetimeId *lifetimeId;

- (BOOL)shouldClose;

@end

NS_ASSUME_NONNULL_END
