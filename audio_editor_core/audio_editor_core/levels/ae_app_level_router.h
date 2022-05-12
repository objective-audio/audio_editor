//
//  ae_app_level_router.h
//

#pragma once

#include <memory>

namespace yas::ae {
class app_level;

struct app_level_router final {
    [[nodiscard]] static std::shared_ptr<app_level_router> make_shared();

    app_level_router();

    std::shared_ptr<app_level> const &level() const;

    void add_level();

   private:
    std::shared_ptr<app_level> _level;
};
}  // namespace yas::ae
