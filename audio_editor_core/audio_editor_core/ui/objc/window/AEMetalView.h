//
//  AEMetalView.h
//

#include <audio_editor_core/ae_project_lifetime_id.h>
#import <ui/yas_ui_umbrella.h>
#include <string>

NS_ASSUME_NONNULL_BEGIN

@interface AEMetalView : YASUIMetalView

- (void)setupWithProjectLifetimeID:(yas::ae::project_lifetime_id const &)project_lifetime_id;

@end

NS_ASSUME_NONNULL_END
