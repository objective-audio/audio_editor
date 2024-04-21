#import "MarkerNamePresenterObjc.h"
#include <ae-core/global/value_types/ae_project_sub_lifetime_id+objc.h>
#include <cpp-utils/cf_utils.h>
#include <ae-core/marker_name/presenter/ae_marker_name_presenter.hpp>

using namespace yas;
using namespace yas::ae;

@interface MarkerNamePresenterObjc ()

@end

@implementation MarkerNamePresenterObjc {
    std::shared_ptr<marker_name_presenter> _raw_presenter;
}

- (void)setupWithSheetLifetimeId:(ProjectSubLifetimeId *)lifetimeId {
    self->_raw_presenter = marker_name_presenter::make_shared([lifetimeId raw_value]);
}

- (NSString *)name {
    return (__bridge NSString *)to_cf_object(self->_raw_presenter->name());
}

- (void)doneWithName:(NSString *)name {
    self->_raw_presenter->done(to_string((__bridge CFStringRef)name));
}

- (void)cancel {
    self->_raw_presenter->cancel();
}

@end
