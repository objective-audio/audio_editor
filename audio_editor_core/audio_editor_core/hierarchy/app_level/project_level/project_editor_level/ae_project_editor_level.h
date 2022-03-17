//
//  ae_project_editor_level.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <cpp_utils/yas_identifier.h>

#include <memory>
#include <string>

namespace yas::ae {
class file_info;
class project_url;
class timing;
class nudging;
class file_track;
class marker_pool;
class edge_editor;
class pasteboard;
class database;
class exporter;
class time_editor_level_pool;
class project_editor;
class project_editor_responder;

struct project_editor_level final {
    [[nodiscard]] static std::shared_ptr<project_editor_level> make_shared(std::string const &project_id,
                                                                           file_info const &);

    identifier const instance_id;
    std::string const project_id;
    file_info const file_info;

    std::shared_ptr<timing> const timing;
    std::shared_ptr<nudging> const nudging;
    std::shared_ptr<file_track> const file_track;
    std::shared_ptr<marker_pool> const marker_pool;
    std::shared_ptr<edge_editor> const edge_editor;
    std::shared_ptr<pasteboard> const pasteboard;
    std::shared_ptr<database> const database;
    std::shared_ptr<exporter> const exporter;
    std::shared_ptr<time_editor_level_pool> const time_editor_level_pool;
    std::shared_ptr<project_editor> const editor;
    std::shared_ptr<project_editor_responder> const responder;

   private:
    project_editor_level(std::string const &identifier, ae::file_info const &, std::shared_ptr<project_url> const &);
};
}  // namespace yas::ae
