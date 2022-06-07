//
//  ae_app_lifecycle.h
//

#pragma once

#include <memory>

namespace yas::ae {
class app_lifetime;

struct app_lifecycle final {
    [[nodiscard]] static std::shared_ptr<app_lifecycle> make_shared();

    app_lifecycle();

    std::shared_ptr<app_lifetime> const &lifetime() const;

    void add();

   private:
    std::shared_ptr<app_lifetime> _lifetime;
};
}  // namespace yas::ae
