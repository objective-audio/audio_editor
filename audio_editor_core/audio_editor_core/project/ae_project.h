//
//  ae_project.h
//

#pragma once

#include <audio_editor_core/ae_app_dependency.h>
#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_project_types.h>

namespace yas::ae {
struct project final {
    [[nodiscard]] static std::shared_ptr<project> make_shared(std::string const &identifier, url const &file_url);
    [[nodiscard]] static std::shared_ptr<project> make_shared(std::string const &identifier, url const &file_url,
                                                              std::shared_ptr<project_url_interface> const &,
                                                              std::shared_ptr<project_file_importer_interface> const &,
                                                              std::shared_ptr<project_file_loader_interface> const &);

    [[nodiscard]] std::string const &identifier() const;
    [[nodiscard]] url const &file_url() const;
    [[nodiscard]] project_state const &state() const;
    [[nodiscard]] std::optional<file_info> file_info() const;

    [[nodiscard]] bool can_close() const;
    void request_close();

    [[nodiscard]] observing::syncable observe_state(std::function<void(project_state const &)> &&);
    [[nodiscard]] observing::syncable observe_file_info(std::function<void(std::optional<ae::file_info>)> &&);
    [[nodiscard]] observing::endable observe_event(std::function<void(project_event const &)> &&);

   private:
    std::string const _identifier;
    url const _file_url;

    std::shared_ptr<project_url_interface> const _project_url;
    std::shared_ptr<project_file_importer_interface> const _file_importer;
    std::shared_ptr<project_file_loader_interface> _file_loader;

    observing::value::holder_ptr<project_state> const _state;
    observing::value::holder_ptr<std::optional<ae::file_info>> const _file_info;

    observing::notifier_ptr<project_event> const _event_notifier;

    project(std::string const &identifier, url const &file_url, std::shared_ptr<project_url_interface> const &,
            std::shared_ptr<project_file_importer_interface> const &,
            std::shared_ptr<project_file_loader_interface> const &);

    project(project const &) = delete;
    project(project &&) = delete;
    project &operator=(project const &) = delete;
    project &operator=(project &&) = delete;

    void _setup(std::weak_ptr<project>);
};
}  // namespace yas::ae
