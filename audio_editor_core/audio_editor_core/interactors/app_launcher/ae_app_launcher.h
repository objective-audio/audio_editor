//
//  ae_app_launcher.h
//

#pragma once

#include <cpp_utils/yas_worker.h>

namespace yas::ae {
class system_url;

struct app_launcher final {
    [[nodiscard]] static std::shared_ptr<app_launcher> make_shared(std::shared_ptr<workable> const &,
                                                                   std::shared_ptr<ae::system_url> const &);

    app_launcher(std::shared_ptr<workable> const &, std::shared_ptr<ae::system_url> const &);

    void launch();

   private:
    std::weak_ptr<workable> _worker;
    std::weak_ptr<ae::system_url> _system_url;
};
}  // namespace yas::ae
