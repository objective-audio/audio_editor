//
//  AEModuleNameViewController.mm
//

#import "ModuleNamePresenterObjc.h"
#include <ae-core/global/value_types/project_sub_lifetime_id+objc.h>
#include <ae-core/module_name/presenter/module_name_presenter.h>
#include <cpp-utils/cf_utils.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct module_name_vc_cpp {
    std::shared_ptr<module_name_presenter> presenter;
};
}  // namespace yas::ae

@implementation ModuleNamePresenterObjc {
    module_name_vc_cpp _cpp;
}

- (void)setupWithSheetLifetimeId:(ProjectSubLifetimeId *)lifetimeId {
    self->_cpp.presenter = module_name_presenter::make_shared([lifetimeId raw_value]);
}

- (NSString *)name {
    return (__bridge NSString *)to_cf_object(self->_cpp.presenter->name());
}

- (void)doneWithName:(NSString *)name {
    self->_cpp.presenter->done(to_string((__bridge CFStringRef)name));
}

- (void)cancel {
    self->_cpp.presenter->cancel();
}

@end
