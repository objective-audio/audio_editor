//
//  AppSettingsWindowPresenter.h
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class AppSettingsLifetimeId;

@interface AppSettingsWindowPresenter : NSObject

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithLifetimeId:(AppSettingsLifetimeId *)lifetimeId;

@property (nonatomic, readonly) AppSettingsLifetimeId *lifetimeId;

- (BOOL)shouldClose;

@end

NS_ASSUME_NONNULL_END
