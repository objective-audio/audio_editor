//
//  ae_project_maker.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_project_pool_dependency.h>
#include <cpp_utils/yas_url.h>

#include <memory>

namespace yas::ae {
class project;

struct project_maker final : project_maker_for_project_pool {
    [[nodiscard]] std::shared_ptr<project_for_project_pool> make(url const &file_url);

    [[nodiscard]] static std::shared_ptr<project_maker> make_shared();
    [[nodiscard]] static std::shared_ptr<project_maker> make_shared(
        std::shared_ptr<uuid_generator_for_project_maker> const &);

   private:
    std::shared_ptr<uuid_generator_for_project_maker> const _uuid_generator;

    project_maker(std::shared_ptr<uuid_generator_for_project_maker> const &);

    project_maker(project_maker const &) = delete;
    project_maker(project_maker &&) = delete;
    project_maker &operator=(project_maker const &) = delete;
    project_maker &operator=(project_maker &&) = delete;
};
}  // namespace yas::ae
