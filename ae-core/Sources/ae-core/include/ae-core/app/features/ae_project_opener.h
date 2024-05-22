//
//  ae_project_opener.h
//

#pragma once

#include <ae-core/app/features/ae_project_opener_event.h>

#include <filesystem>
#include <memory>
#include <observing/umbrella.hpp>

namespace yas::ae {
class file_info_loader;
class project_lifecycle;
class project_format;

struct project_opener final {
    project_opener(file_info_loader const *, project_lifecycle *);

    void open(project_format const &, std::filesystem::path const &project_path);

    [[nodiscard]] observing::endable observe_event(std::function<void(project_opener_event const &)> &&);

   private:
    file_info_loader const *const _file_info_loader;
    project_lifecycle *const _project_lifecycle;

    observing::notifier_ptr<project_opener_event> const _event_notifier;

    project_opener(project_opener const &) = delete;
    project_opener(project_opener &&) = delete;
    project_opener &operator=(project_opener const &) = delete;
    project_opener &operator=(project_opener &&) = delete;
};
}  // namespace yas::ae
