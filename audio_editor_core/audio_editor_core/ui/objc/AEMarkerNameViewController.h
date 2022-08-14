//
//  AEMarkerNameViewController.h
//

#import <Cocoa/Cocoa.h>
#include <audio_editor_core/ae_window_sub_lifetime_id.h>

NS_ASSUME_NONNULL_BEGIN

namespace yas::ae {
enum class marker_name_vc_event {
    done,
    canceled,
};
}

@interface AEMarkerNameViewController : NSViewController

+ (instancetype)instantiateWithSheetLifetimeId:(yas::ae::sheet_lifetime_id const &)sheet_lifetime_id
                                   markerFrame:(int64_t const)marker_frame;

@end

NS_ASSUME_NONNULL_END
