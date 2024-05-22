#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <ae-core/app/value_types/AppWindowAction.h>
#import <ae-core/global/value_types/AppSettingsLifetimeId.h>
#import <ae-core/global/value_types/ProjectLifetimeId.h>

NS_ASSUME_NONNULL_BEGIN

@interface AppWindowPresenterObjc : NSObject

- (BOOL)respondsToAction:(AppWindowAction const)action_kind;
- (void)performAction:(AppWindowAction const)action_kind;

- (void)openProjectSetupDialogWithCompletion:(void (^)(NSModalResponse, NSURL *))completion;

- (void)makeAndShowProjectSettingsWithLifetimeId:(ProjectLifetimeId *)lifetimeId;
- (void)disposeProjectSettingsWithLifetimeId:(ProjectLifetimeId *)lifetimeId;
- (void)makeAndShowAppSettingsWithLifetimeId:(AppSettingsLifetimeId *)lifetimeId;
- (void)disposeAppSettingsWithLifetimeId:(AppSettingsLifetimeId *)lifetimeId;

- (void)makeAndShowProjectWithLifetimeId:(ProjectLifetimeId *)lifetimeId;
- (void)showProjectWithLifetimeId:(ProjectLifetimeId *)lifetimeId;
- (void)disposeProjectWithLifetimeId:(ProjectLifetimeId *)lifetimeId;

@end

NS_ASSUME_NONNULL_END
