#import <Cocoa/Cocoa.h>
#include <ae-core/global/value_types/ProjectSubLifetimeId.h>

NS_ASSUME_NONNULL_BEGIN

@interface MarkerNamePresenterObjc : NSObject

@property (nonatomic, readonly) NSString *name;

- (void)setupWithSheetLifetimeId:(ProjectSubLifetimeId *)lifetimeId;
- (void)doneWithName:(NSString *)name;
- (void)cancel;

@end

NS_ASSUME_NONNULL_END
