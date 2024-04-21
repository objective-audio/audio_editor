#import "AppDelegatePresenter.h"
#import <ae-core/app/AppWindowPresenterObjc.h>
#include <ae-core/global/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

@implementation AppDelegatePresenter

- (void)didFinishLaunching {
    hierarchy::app_lifecycle()->add();
}

- (AppWindowAction)actionForSelector:(SEL)selector {
    if (selector == @selector(openDocument:)) {
        return AppWindowActionOpenProject;
    } else if (selector == @selector(openAppSettings:)) {
        return AppWindowActionOpenAppSettings;
    } else {
        return AppWindowActionUnknown;
    }
}

@end
