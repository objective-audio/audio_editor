//
//  ae_app.cpp
//

#include "ae_app.h"

#include <audio_editor_core/ae_project.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

app::app() {
}

app_ptr app::make_shared() {
    return std::shared_ptr<app>(new app{});
}

app_ptr app::global() {
    static app_ptr const _global = app::make_shared();
    return _global;
}

project_pool_ptr const &app::project_pool() const {
    return this->_project_pool;
}
