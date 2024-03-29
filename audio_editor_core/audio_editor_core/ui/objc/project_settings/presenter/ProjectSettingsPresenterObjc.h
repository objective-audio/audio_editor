//
//  ProjectSettingsPresenterObjc.h
//

#pragma once

#import <Foundation/Foundation.h>
#import <audio_editor_core/ae_timing_types_objc.h>

NS_ASSUME_NONNULL_BEGIN

@class ProjectLifetimeId;

@interface ProjectSettingsPresenterObjc : NSObject

- (instancetype)initWithProjectLifetimeId:(ProjectLifetimeId *)lifetime_id;

@property (nonatomic, readonly) NSInteger sampleRate;
@property (nonatomic, readonly) NSInteger channelCount;

@end

NS_ASSUME_NONNULL_END
