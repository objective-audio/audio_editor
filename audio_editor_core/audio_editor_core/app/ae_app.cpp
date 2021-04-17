//
//  ae_app.cpp
//

#include "ae_app.h"

using namespace yas;
using namespace yas::ae;

app::app() {
}

app_ptr app::make_shared() {
    return std::shared_ptr<app>(new app{});
}
