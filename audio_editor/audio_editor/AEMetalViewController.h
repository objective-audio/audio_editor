//
//  AEMetalViewController.h
//

#import <ui/ui.h>
#include <string>

NS_ASSUME_NONNULL_BEGIN

@interface AEMetalViewController : YASUIMetalViewController

- (void)setupWithProjectID:(std::string const &)project_id;

@end

NS_ASSUME_NONNULL_END
