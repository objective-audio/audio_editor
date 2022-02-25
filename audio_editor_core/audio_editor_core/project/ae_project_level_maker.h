//
//  ae_project_level_maker.h
//

#pragma once

#include <cpp_utils/yas_url.h>

namespace yas::ae {
class project_level;
class uuid_generatable;

struct project_level_maker final {
    [[nodiscard]] std::shared_ptr<project_level> make(url const &file_url);

    [[nodiscard]] static std::shared_ptr<project_level_maker> make_shared();
    [[nodiscard]] static std::shared_ptr<project_level_maker> make_shared(std::shared_ptr<uuid_generatable> const &);

   private:
    std::shared_ptr<uuid_generatable> const _uuid_generator;

    project_level_maker(std::shared_ptr<uuid_generatable> const &);

    project_level_maker(project_level_maker const &) = delete;
    project_level_maker(project_level_maker &&) = delete;
    project_level_maker &operator=(project_level_maker const &) = delete;
    project_level_maker &operator=(project_level_maker &&) = delete;
};
}  // namespace yas::ae
