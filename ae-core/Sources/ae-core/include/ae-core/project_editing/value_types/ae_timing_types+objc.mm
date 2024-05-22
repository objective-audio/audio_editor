//
//  ae_timing_types+objc.cpp
//

#include "ae_timing_types+objc.h"

using namespace yas;
using namespace yas::ae;

ae::timing_fraction_kind yas::to_cpp(TimingFractionKind const objc_kind) {
    switch (objc_kind) {
        case TimingFractionKindSample:
            return timing_fraction_kind::sample;
        case TimingFractionKindMilisecond:
            return timing_fraction_kind::milisecond;
        case TimingFractionKindFrame30:
            return timing_fraction_kind::frame30;
    }
}

TimingFractionKind yas::to_objc(ae::timing_fraction_kind const cpp_kind) {
    switch (cpp_kind) {
        case timing_fraction_kind::sample:
            return TimingFractionKindSample;
        case timing_fraction_kind::milisecond:
            return TimingFractionKindMilisecond;
        case timing_fraction_kind::frame30:
            return TimingFractionKindFrame30;
    }
}

ae::timing_unit_kind yas::to_cpp(TimingUnitKind const objc_kind) {
    switch (objc_kind) {
        case TimingUnitKindFraction:
            return timing_unit_kind::fraction;
        case TimingUnitKindSeconds:
            return timing_unit_kind::seconds;
        case TimingUnitKindMinutes:
            return timing_unit_kind::minutes;
        case TimingUnitKindHours:
            return timing_unit_kind::hours;
    }
}

TimingUnitKind yas::to_objc(ae::timing_unit_kind const cpp_kind) {
    switch (cpp_kind) {
        case timing_unit_kind::fraction:
            return TimingUnitKindFraction;
        case timing_unit_kind::seconds:
            return TimingUnitKindSeconds;
        case timing_unit_kind::minutes:
            return TimingUnitKindMinutes;
        case timing_unit_kind::hours:
            return TimingUnitKindHours;
    }
}
