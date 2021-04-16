//
//  ae_app.h
//

#pragma once

#include <memory>

namespace yas::ae {
class ae_app;
using ae_app_ptr = std::shared_ptr<ae_app>;

struct ae_app {
  ae_app_ptr make_shared();

private:
  ae_app();

  ae_app(ae_app const &) = delete;
  ae_app(ae_app &&) = delete;
  ae_app &operator=(ae_app const &) = delete;
  ae_app &operator=(ae_app &&) = delete;
};
} // namespace yas::ae
