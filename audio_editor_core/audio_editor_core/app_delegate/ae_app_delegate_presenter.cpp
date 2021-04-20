//
//  ae_app_delegate_presenter.cpp
//

#include "ae_app_delegate_presenter.h"

using namespace yas;
using namespace yas::ae;

app_delegate_presenter::app_delegate_presenter() {
}

std::shared_ptr<app_delegate_presenter> app_delegate_presenter::make_shared() {
    return std::shared_ptr<app_delegate_presenter>(new app_delegate_presenter{});
}
