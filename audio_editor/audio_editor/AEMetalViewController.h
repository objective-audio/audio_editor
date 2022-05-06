//
//  AEMetalViewController.h
//

#include <audio_editor_core/ae_project_id.h>
#import <ui/ui.h>
#include <string>

NS_ASSUME_NONNULL_BEGIN

@interface AEMetalViewController : YASUIMetalViewController

- (void)setupWithProjectID:(yas::ae::project_id const &)project_id;

@end

NS_ASSUME_NONNULL_END
