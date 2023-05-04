//
//  ProjectSettingsPresenterObjc.h
//

#pragma once

#import <Foundation/Foundation.h>
#import <audio_editor_core/ae_timing_types_objc.h>

NS_ASSUME_NONNULL_BEGIN

@class WindowLifetimeId;

@interface ProjectSettingsPresenterObjc : NSObject

- (instancetype)initWithWindowLifetimeId:(WindowLifetimeId *)lifetime_id;

@property (nonatomic, readonly) NSInteger sampleRate;
@property (nonatomic, readonly) NSInteger channelCount;

@end

NS_ASSUME_NONNULL_END
