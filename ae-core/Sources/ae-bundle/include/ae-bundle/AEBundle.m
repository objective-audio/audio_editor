#import "AEBundle.h"
@import ae_swift_bundle;

@implementation AEBundle

+ (nullable NSColor *)colorWithName:(NSString *)name {
    return [AESwiftBundle colorWithName:name error:nil];
}

@end
