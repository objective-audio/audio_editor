//
//  AEWindowController.h
//

#import <Cocoa/Cocoa.h>
#include <audio_editor_core/audio_editor_core_umbrella.h>

NS_ASSUME_NONNULL_BEGIN

@interface AEWindowController : NSWindowController

@property (nonatomic, readonly) yas::ae::app_project_interface_ptr project;

- (void)setupWithProject:(yas::ae::app_project_interface_ptr const &)project;

@end

NS_ASSUME_NONNULL_END
