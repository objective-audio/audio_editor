#import "ProjectWindowPresenter.h"
#include <ae-core/global/value_types/ae_project_lifetime_id+objc.h>
#include <ae-core/window/presenter/ae_project_presenter.h>
#include <cpp-utils/cf_utils.h>
#include <cpp-utils/unowned.h>

using namespace yas;
using namespace yas::ae;

@implementation ProjectWindowPresenter {
    std::shared_ptr<project_presenter> _raw_presenter;
    observing::canceller_pool _pool;
}

- (instancetype)initWithLifetimeId:(ProjectLifetimeId *)lifetimeId {
    self = [super init];
    if (self) {
        _lifetimeId = lifetimeId;
        _raw_presenter = project_presenter::make_shared([lifetimeId raw_value]);
    }
    return self;
}

- (NSString *)title {
    return (__bridge NSString *)to_cf_object(_raw_presenter->title());
}

- (NSString *)autoSaveName {
    auto const name = _raw_presenter->auto_save_name();
    if (name.has_value()) {
        return (__bridge NSString *)to_cf_object(name.value());
    } else {
        return nil;
    }
}

- (NSURL *)representedUrl {
    auto const url = _raw_presenter->represented_url();
    if (url.has_value()) {
        return (__bridge NSURL *)url.value().cf_url();
    } else {
        return nil;
    }
}

- (BOOL)shouldClose {
    return self->_raw_presenter->should_close();
}

- (void)observe:(void (^)(void))handler {
    auto const lifetime_id = _raw_presenter->lifetime_id;

    self->_raw_presenter
        ->observe([lifetime_id, handler = std::move(handler)](project_presenter_event const &event) { handler(); })
        .sync()
        ->add_to(self->_pool);
}

@end
