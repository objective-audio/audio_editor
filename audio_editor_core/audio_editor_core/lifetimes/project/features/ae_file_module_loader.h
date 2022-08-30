//
//  ae_file_module_loader.h
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>

#include <filesystem>
#include <memory>

namespace yas::ae {
class project_url;
class file_importer;
class file_info_loader;
class file_module_loading_state_holder;
class database;
class file_track;
class edge_holder;
class player;
class timeline_holder;
class uuid_generatable;

struct file_module_loader final : std::enable_shared_from_this<file_module_loader> {
    [[nodiscard]] static std::shared_ptr<file_module_loader> make_shared(
        project_id const &project_id, project_url const *project_url, project_format const &project_format,
        player *player, file_module_loading_state_holder *state_holder, database *database, file_track *file_track,
        edge_holder *edge_holder, timeline_holder const *timeline_holder);

    file_module_loader(std::shared_ptr<uuid_generatable> const &, project_id const &, project_url const *,
                       project_format const &, file_importer *, file_info_loader const *, player *,
                       file_module_loading_state_holder *, database *, file_track *, edge_holder *,
                       timeline_holder const *);

    void load(std::filesystem::path const &src_path);

   private:
    std::shared_ptr<uuid_generatable> const _uuid_generator;
    project_id const _project_id;
    project_url const *const _project_url;
    project_format const _project_format;
    file_importer *const _file_importer;
    file_info_loader const *const _file_info_loader;
    player *const _player;
    file_module_loading_state_holder *const _state_holder;
    database *const _database;
    file_track *const _file_track;
    edge_holder *const _edge_holder;
    timeline_holder const *const _timeline_holder;
};
}  // namespace yas::ae
