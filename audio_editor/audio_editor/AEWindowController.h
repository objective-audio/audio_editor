//
//  AEWindowController.h
//

#import <Cocoa/Cocoa.h>
#include <string>

NS_ASSUME_NONNULL_BEGIN

@interface AEWindowController : NSWindowController

- (void)setupWithProjectID:(std::string const &)project_id;
- (std::string const &)project_id;

@end

NS_ASSUME_NONNULL_END
