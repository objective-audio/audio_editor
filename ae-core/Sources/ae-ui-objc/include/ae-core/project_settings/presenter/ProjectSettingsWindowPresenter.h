//
//  ProjectSettingsWindowPresenter.h
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ProjectLifetimeId;

@interface ProjectSettingsWindowPresenter : NSObject

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithLifetimeId:(ProjectLifetimeId *)lifetimeId;

@property (nonatomic, readonly) ProjectLifetimeId *lifetimeId;

- (BOOL)shouldClose;

@end

NS_ASSUME_NONNULL_END
