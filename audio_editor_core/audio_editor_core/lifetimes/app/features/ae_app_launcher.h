//
//  ae_app_launcher.h
//

#pragma once

#include <cpp_utils/yas_worker.h>

namespace yas::ae {
class system_path;

struct app_launcher final {
    app_launcher(std::shared_ptr<workable> const &, std::shared_ptr<ae::system_path> const &);

    void launch();

   private:
    std::weak_ptr<workable> _worker;
    std::weak_ptr<ae::system_path> _system_path;

    app_launcher(app_launcher const &) = delete;
    app_launcher(app_launcher &&) = delete;
    app_launcher &operator=(app_launcher const &) = delete;
    app_launcher &operator=(app_launcher &&) = delete;
};
}  // namespace yas::ae
