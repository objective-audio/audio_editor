#import <Foundation/Foundation.h>
#import <ae-core/app/value_types/AppWindowAction.h>

NS_ASSUME_NONNULL_BEGIN

@interface AppDelegatePresenter : NSObject

- (void)didFinishLaunching;

- (AppWindowAction)actionForSelector:(SEL)selector;

@end

NS_ASSUME_NONNULL_END
