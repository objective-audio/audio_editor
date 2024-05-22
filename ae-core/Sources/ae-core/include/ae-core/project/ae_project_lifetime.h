//
//  ae_project_lifetime.h
//

#pragma once

#include <ae-core/app/value_types/ae_project_format.h>
#include <ae-core/global/value_types/ae_project_lifetime_id.h>

#include <filesystem>
#include <memory>

namespace yas::ae {
class app_lifetime;
class project_sub_lifecycle;
class project_closer;
class project_path;
class project_receiver;
class project_state_holder;
class zooming_pair;
class horizontal_scrolling;
class vertical_scrolling;
class player;
class timeline_holder;
class display_space;

struct project_lifetime final {
    [[nodiscard]] static std::shared_ptr<project_lifetime> make_shared(project_lifetime_id const &,
                                                                       project_format const &,
                                                                       std::filesystem::path const &project_dir_path);

    project_lifetime(project_lifetime_id const &, project_format const &, std::filesystem::path const &project_dir_path,
                     app_lifetime const *);

    project_lifetime_id const lifetime_id;
    project_format const project_format;
    std::filesystem::path const project_directory_path;

    std::shared_ptr<project_path> const project_path;
    std::shared_ptr<display_space> const display_space;
    std::shared_ptr<ae::zooming_pair> const zooming_pair;
    std::shared_ptr<ae::horizontal_scrolling> const horizontal_scrolling;
    std::shared_ptr<ae::vertical_scrolling> const vertical_scrolling;
    std::shared_ptr<ae::player> const player;
    std::shared_ptr<timeline_holder> const timeline_holder;
    std::shared_ptr<project_sub_lifecycle> const project_sub_lifecycle;
    std::shared_ptr<project_closer> const closer;
    std::shared_ptr<project_receiver> const receiver;

   private:
    project_lifetime(project_lifetime const &) = delete;
    project_lifetime(project_lifetime &&) = delete;
    project_lifetime &operator=(project_lifetime const &) = delete;
    project_lifetime &operator=(project_lifetime &&) = delete;
};
}  // namespace yas::ae
