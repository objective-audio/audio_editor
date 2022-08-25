//
//  AEMarkerNameViewController.h
//

#import <Cocoa/Cocoa.h>
#include <audio_editor_core/ae_project_sub_lifetime_id.h>

NS_ASSUME_NONNULL_BEGIN

namespace yas::ae {
enum class marker_name_vc_event {
    done,
    canceled,
};
}

@interface AEMarkerNameViewController : NSViewController

+ (instancetype)instantiateWithSheetLifetimeId:(yas::ae::sheet_lifetime_id const &)sheet_lifetime_id;

@end

NS_ASSUME_NONNULL_END
