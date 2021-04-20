//
//  ae_app_global.cpp
//

#include "ae_app_global.h"

using namespace yas;
using namespace yas::ae;

app_ptr ae::app_global() {
    static app_ptr const _app_global = app::make_shared();
    return _app_global;
}
