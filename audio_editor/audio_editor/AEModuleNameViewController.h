//
//  AEModuleNameViewController.h
//

#import <Cocoa/Cocoa.h>
#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <observing/yas_observing_umbrella.h>

NS_ASSUME_NONNULL_BEGIN

namespace yas::ae {
enum class module_name_vc_event {
    done,
    canceled,
};
}

@interface AEModuleNameViewController : NSViewController

+ (instancetype)instantiateWithWindowLifetimeId:(yas::ae::window_lifetime_id const &)window_lifetime_id
                                    moduleRange:(yas::ae::time::range const)module_range;

@end

NS_ASSUME_NONNULL_END
