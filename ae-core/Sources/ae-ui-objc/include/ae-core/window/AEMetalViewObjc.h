//
//  AEMetalView.h
//

#pragma once

#import <ae-core/global/value_types/ProjectLifetimeId.h>
#include <ui/metal/view/yas_ui_metal_view_objc.h>

NS_ASSUME_NONNULL_BEGIN

@interface AEMetalViewObjc : YASUIMetalView

- (void)setupWithProjectLifetimeId:(ProjectLifetimeId *)lifetimeId;

@end

NS_ASSUME_NONNULL_END
