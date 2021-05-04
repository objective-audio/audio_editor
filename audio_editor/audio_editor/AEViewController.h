//
//  AEViewController.h
//

#import <Cocoa/Cocoa.h>
#include <string>

@interface AEViewController : NSViewController

- (void)setupWithProjectID:(std::string const &)project_id;

@end
