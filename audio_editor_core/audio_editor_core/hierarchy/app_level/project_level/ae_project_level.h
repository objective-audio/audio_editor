//
//  ae_project_level.h
//

#pragma once

#include <memory>
#include <string>

namespace yas::ae {
struct project_level final {
    [[nodiscard]] static std::shared_ptr<project_level> make_shared(std::string const &identifier);

    std::string const identifier;

   private:
    project_level(std::string const &identifier);
};
}  // namespace yas::ae
