//
//  ae_app.hpp
//

#pragma once

#include <cpp_utils/yas_worker.h>

namespace yas::ae {
class system_url;

struct app final {
    [[nodiscard]] static std::shared_ptr<app> make_shared(std::shared_ptr<ae::system_url> const &);

    worker_ptr const worker;

    void setup();

   private:
    std::shared_ptr<ae::system_url> const _system_url;

    app(std::shared_ptr<ae::system_url> const &);
};
}  // namespace yas::ae
