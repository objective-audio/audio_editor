#pragma once

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_CLOSED_ENUM(NSInteger, AppWindowAction) {
    AppWindowActionUnknown,

    AppWindowActionOpenProject,
    AppWindowActionOpenAppSettings,
};

NS_ASSUME_NONNULL_END
