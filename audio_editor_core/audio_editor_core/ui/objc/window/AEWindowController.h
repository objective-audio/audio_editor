//
//  AEWindowController.h
//

#import <Cocoa/Cocoa.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <string>

NS_ASSUME_NONNULL_BEGIN

@interface AEWindowController : NSWindowController

+ (instancetype)instantiateWithLifetimeID:(yas::ae::window_lifetime_id const &)lifetime_id;

- (void)setupWithLifetimeID:(yas::ae::window_lifetime_id const &)lifetime_id;
- (yas::ae::window_lifetime_id const &)lifetime_id;

@end

NS_ASSUME_NONNULL_END
