//
//  ae_app_settings.mm
//

#include "ae_app_settings.hpp"
#import <Foundation/Foundation.h>
#include <string>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
namespace app_settings_constants {
    namespace key {
        static NSString *const timing_fraction_kind = @"timing_fraction_kind";
        static NSString *const timing_unit_kind = @"timing_unit_kind";
    }

    namespace value {
        namespace timing_fraction {
            static NSString *const sample = @"sample";
            static NSString *const milisecond = @"milisecond";
            static NSString *const frame30 = @"frame30";
        }

        namespace timing_unit {
            static NSString *const fraction = @"fraction";
            static NSString *const seconds = @"seconds";
            static NSString *const minutes = @"minutes";
            static NSString *const hours = @"hours";
        }
    }
}
}

using namespace yas::ae::app_settings_constants;

namespace yas::ae::app_settings_utils {
timing_fraction_kind to_fraction_kind(NSString *value) {
    if ([value isEqualToString:value::timing_fraction::frame30]) {
        return timing_fraction_kind::frame30;
    } else if ([value isEqualToString:value::timing_fraction::milisecond]) {
        return timing_fraction_kind::milisecond;
    } else {
        return timing_fraction_kind::sample;
    }
}

NSString *to_value(timing_fraction_kind const kind) {
    switch (kind) {
        case timing_fraction_kind::sample:
            return value::timing_fraction::sample;
        case timing_fraction_kind::milisecond:
            return value::timing_fraction::milisecond;
        case timing_fraction_kind::frame30:
            return value::timing_fraction::frame30;
    }
}

timing_unit_kind to_unit_kind(NSString *key) {
    if ([key isEqualToString:value::timing_unit::hours]) {
        return timing_unit_kind::hours;
    } else if ([key isEqualToString:value::timing_unit::minutes]) {
        return timing_unit_kind::minutes;
    } else if ([key isEqualToString:value::timing_unit::seconds]) {
        return timing_unit_kind::seconds;
    } else {
        return timing_unit_kind::fraction;
    }
}

NSString *to_value(timing_unit_kind const kind) {
    switch (kind) {
        case timing_unit_kind::hours:
            return value::timing_unit::hours;
        case timing_unit_kind::minutes:
            return value::timing_unit::minutes;
        case timing_unit_kind::seconds:
            return value::timing_unit::seconds;
        case timing_unit_kind::fraction:
            return value::timing_unit::fraction;
    }
}
}

app_settings::app_settings() {
    [NSUserDefaults.standardUserDefaults registerDefaults:@{
        key::timing_unit_kind: value::timing_unit::fraction,
        key::timing_fraction_kind: value::timing_fraction::sample
    }];
}

void app_settings::set_timing_fraction_kind(ae::timing_fraction_kind const kind) {
    NSString *value = app_settings_utils::to_value(kind);
    [NSUserDefaults.standardUserDefaults setObject:value forKey:key::timing_fraction_kind];
}

ae::timing_fraction_kind app_settings::timing_fraction_kind() const {
    NSString *kind_string = [NSUserDefaults.standardUserDefaults stringForKey:key::timing_fraction_kind];
    return app_settings_utils::to_fraction_kind(kind_string);
}

void app_settings::set_timing_unit_kind(ae::timing_unit_kind const kind) {
    NSString *value = app_settings_utils::to_value(kind);
    [NSUserDefaults.standardUserDefaults setObject:value forKey:key::timing_unit_kind];
}

ae::timing_unit_kind app_settings::timing_unit_kind() const {
    NSString *kind_string = [NSUserDefaults.standardUserDefaults stringForKey:key::timing_unit_kind];
    return app_settings_utils::to_unit_kind(kind_string);
}
