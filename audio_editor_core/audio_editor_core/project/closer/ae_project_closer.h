//
//  ae_project_closer.h
//

#pragma once

#include <memory>

namespace yas::ae {
struct project_closer final {
    [[nodiscard]] static std::shared_ptr<project_closer> make_shared();

   private:
    project_closer();
};
}  // namespace yas::ae
