//
//  ae_app.h
//

#pragma once

#include <memory>

namespace yas::ae {
class app;
using app_ptr = std::shared_ptr<app>;

struct app {
    static app_ptr make_shared();

   private:
    app();

    app(app const &) = delete;
    app(app &&) = delete;
    app &operator=(app const &) = delete;
    app &operator=(app &&) = delete;
};
}  // namespace yas::ae
