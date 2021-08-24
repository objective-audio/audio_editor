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
    [[nodiscard]] static std::shared_ptr<project> make_shared(
        std::string const &identifier, url const &file_url, std::shared_ptr<project_url_for_project> const &,
        std::shared_ptr<file_importer_for_project> const &, std::shared_ptr<file_loader_for_project> const &,
        std::shared_ptr<player_for_project> const &, std::shared_ptr<project_editor_maker_for_project> const &);

    [[nodiscard]] std::string const &identifier() const;
    [[nodiscard]] url const &file_url() const;
    [[nodiscard]] project_state const &state() const;
    [[nodiscard]] std::optional<file_info> file_info() const;
    [[nodiscard]] std::shared_ptr<player_for_project> const &player() const;
    [[nodiscard]] std::shared_ptr<project_editor_for_project> const &editor() const;

    [[nodiscard]] bool can_close() const;
    void request_close();

    [[nodiscard]] bool can_nudge() const;
    void nudge_previous();
    void nudge_next();

    [[nodiscard]] bool can_jump_to_previous_edge() const;
    [[nodiscard]] bool can_jump_to_next_edge() const;
    void jump_to_previous_edge();
    void jump_to_next_edge();

    [[nodiscard]] bool can_split() const;
    void split();
    void drop_head_and_offset();
    void drop_tail_and_offset();

    [[nodiscard]] bool can_erase() const;
    void erase();

    [[nodiscard]] bool can_insert_marker() const;
    void insert_marker();

    [[nodiscard]] bool can_return_to_zero() const;
    void return_to_zero();

    [[nodiscard]] observing::syncable observe_state(std::function<void(project_state const &)> &&);
    [[nodiscard]] observing::syncable observe_file_info(std::function<void(std::optional<ae::file_info>)> &&);
    [[nodiscard]] observing::endable observe_event(std::function<void(project_event const &)> &&);

   private:
    std::string const _identifier;
    url const _file_url;

    std::shared_ptr<project_url_for_project> const _project_url;
    std::shared_ptr<file_importer_for_project> const _file_importer;
    std::shared_ptr<file_loader_for_project> const _file_loader;
    std::shared_ptr<player_for_project> const _player;
    std::shared_ptr<project_editor_maker_for_project> const _editor_maker;
    std::shared_ptr<project_editor_for_project> _editor = nullptr;

    observing::value::holder_ptr<project_state> const _state;
    observing::value::holder_ptr<std::optional<ae::file_info>> const _file_info;

    observing::notifier_ptr<project_event> const _event_notifier;

    project(std::string const &identifier, url const &file_url, std::shared_ptr<project_url_for_project> const &,
            std::shared_ptr<file_importer_for_project> const &, std::shared_ptr<file_loader_for_project> const &,
            std::shared_ptr<player_for_project> const &, std::shared_ptr<project_editor_maker_for_project> const &);

    project(project const &) = delete;
    project(project &&) = delete;
    project &operator=(project const &) = delete;
    project &operator=(project &&) = delete;

    void _setup(std::weak_ptr<project>);
};
}  // namespace yas::ae
