//
//  AEWindowController.h
//

#import <Cocoa/Cocoa.h>
#include <audio_editor_core/audio_editor_core_umbrella.h>

NS_ASSUME_NONNULL_BEGIN

@interface AEWindowController : NSWindowController

@property (nonatomic, readonly) yas::ae::project_ptr project;

- (void)setupWithProject:(yas::ae::project_ptr const &)project;

@end

NS_ASSUME_NONNULL_END
