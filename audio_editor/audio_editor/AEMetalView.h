//
//  AEMetalView.h
//

#import <ui/ui.h>
#include <string>

NS_ASSUME_NONNULL_BEGIN

@interface AEMetalView : YASUIMetalView

- (void)setupWithProjectID:(std::string const &)project_id;

@end

NS_ASSUME_NONNULL_END
