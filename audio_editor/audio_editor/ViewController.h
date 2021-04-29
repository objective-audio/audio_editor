//
//  ViewController.h
//

#import <Cocoa/Cocoa.h>
#include <string>

@interface ViewController : NSViewController

- (void)setupWithProjectID:(std::string const &)project_id;

@end
