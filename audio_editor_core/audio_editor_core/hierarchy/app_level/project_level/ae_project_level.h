//
//  ae_project_level.h
//

#pragma once

#include <cpp_utils/yas_url.h>

#include <memory>
#include <string>

namespace yas::ae {
class zooming;
class scrolling;
class project;

struct project_level final {
    [[nodiscard]] static std::shared_ptr<project_level> make_shared(std::string const &identifier, url const &file_url);

    std::string const identifier;

    std::shared_ptr<ae::zooming> const horizontal_zooming;
    std::shared_ptr<ae::zooming> const vertical_zooming;
    std::shared_ptr<ae::scrolling> const scrolling;

    std::shared_ptr<project> const project;

   private:
    project_level(std::string const &identifier, std::shared_ptr<ae::scrolling> const &,
                  std::shared_ptr<ae::project> const &);
};
}  // namespace yas::ae
