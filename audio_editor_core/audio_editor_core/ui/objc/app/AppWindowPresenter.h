//
//  AppWindowPresenter.h
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

namespace yas::ae {
enum class app_window_action_kind {
    open_project,
    open_app_settings,
};
}

@interface AppWindowPresenter : NSObject

- (BOOL)respondsToAction:(yas::ae::app_window_action_kind const)action_kind;
- (void)performAction:(yas::ae::app_window_action_kind)action_kind;

@end

NS_ASSUME_NONNULL_END
