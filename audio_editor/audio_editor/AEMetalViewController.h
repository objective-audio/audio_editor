//
//  AEMetalViewController.h
//

#include <audio_editor_core/ae_window_lifetime_id.h>
#import <ui/ui.h>
#include <string>

NS_ASSUME_NONNULL_BEGIN

@interface AEMetalViewController : YASUIMetalViewController

- (void)setupWithWindowLifetimeID:(yas::ae::window_lifetime_id const &)lifetime_id;

@end

NS_ASSUME_NONNULL_END
