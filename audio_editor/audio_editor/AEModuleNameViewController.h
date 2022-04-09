//
//  AEModuleNameViewController.h
//

#import <Cocoa/Cocoa.h>
#include <audio_editor_core/ae_common_types.h>
#include <observing/yas_observing_umbrella.h>

NS_ASSUME_NONNULL_BEGIN

namespace yas::ae {
enum class module_name_vc_event {
    done,
    canceled,
};
}

@interface AEModuleNameViewController : NSViewController

+ (instancetype)instantiateWithProjectId:(std::string const &)project_id
                             moduleRange:(yas::ae::time::range const)module_range;

- (yas::observing::endable)observe_event:(std::function<void(yas::ae::module_name_vc_event const &)> &&)handler;

@end

NS_ASSUME_NONNULL_END
