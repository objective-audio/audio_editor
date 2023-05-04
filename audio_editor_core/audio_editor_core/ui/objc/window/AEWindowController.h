//
//  AEWindowController.h
//

#import <Cocoa/Cocoa.h>
#include <audio_editor_core/ae_project_lifetime_id.h>

NS_ASSUME_NONNULL_BEGIN

@interface AEWindowController : NSWindowController

+ (instancetype)instantiateWithLifetimeID:(yas::ae::project_lifetime_id const &)lifetime_id;

- (yas::ae::project_lifetime_id const &)lifetime_id;

@end

NS_ASSUME_NONNULL_END
