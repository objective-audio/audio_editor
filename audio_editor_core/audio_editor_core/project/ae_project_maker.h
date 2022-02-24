//
//  ae_project_maker.h
//

#pragma once

#include <cpp_utils/yas_url.h>

namespace yas::ae {
class project_level;
class uuid_generatable;

struct project_maker final {
    [[nodiscard]] std::shared_ptr<project_level> make(url const &file_url);

    [[nodiscard]] static std::shared_ptr<project_maker> make_shared();
    [[nodiscard]] static std::shared_ptr<project_maker> make_shared(std::shared_ptr<uuid_generatable> const &);

   private:
    std::shared_ptr<uuid_generatable> const _uuid_generator;

    project_maker(std::shared_ptr<uuid_generatable> const &);

    project_maker(project_maker const &) = delete;
    project_maker(project_maker &&) = delete;
    project_maker &operator=(project_maker const &) = delete;
    project_maker &operator=(project_maker &&) = delete;
};
}  // namespace yas::ae
