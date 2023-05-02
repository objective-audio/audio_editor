//
//  ae_timing_types_objc.h
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_CLOSED_ENUM(NSInteger, TimingFractionKind) {
    TimingFractionKindSample,
    TimingFractionKindMilisecond,
    TimingFractionKindFrame30,
};

typedef NS_CLOSED_ENUM(NSInteger, TimingUnitKind) {
    TimingUnitKindFraction,
    TimingUnitKindSeconds,
    TimingUnitKindMinutes,
    TimingUnitKindHours,
};

NS_ASSUME_NONNULL_END
