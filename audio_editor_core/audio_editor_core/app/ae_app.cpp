//
//  ae_app.cpp
//

#include "ae_app.h"

using namespace yas;
using namespace yas::ae;

ae_app::ae_app() {}

ae_app_ptr ae_app::make_shared() {
  return std::shared_ptr<ae_app>(new ae_app{});
}
