//
//  AEWindowController.h
//

#import <Cocoa/Cocoa.h>
#include <audio_editor_core/ae_project_id.h>
#include <string>

NS_ASSUME_NONNULL_BEGIN

@interface AEWindowController : NSWindowController

- (void)setupWithProjectID:(yas::ae::project_id const &)project_id;
- (yas::ae::project_id const &)project_id;

@end

NS_ASSUME_NONNULL_END
