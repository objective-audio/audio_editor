#import <Foundation/Foundation.h>
#import <ae-core/project_editing/value_types/ae_timing_types_objc.h>

NS_ASSUME_NONNULL_BEGIN

@interface AppSettingsPresenterObjc : NSObject

@property (nonatomic) TimingFractionKind timingFractionKind;
@property (nonatomic) TimingUnitKind timingUnitKind;

- (void)observe:(void (^)())handler;

@end

NS_ASSUME_NONNULL_END
