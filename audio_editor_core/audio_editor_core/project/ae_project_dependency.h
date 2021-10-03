//
//  ae_project_dependency.h
//

#pragma once

#include <audio_editor_core/ae_file_importer_types.h>
#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_player_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

#include <optional>

namespace yas::proc {
class timeline;
}

namespace yas::ae {
struct project_url_for_project {
    virtual ~project_url_for_project() = default;

    [[nodiscard]] virtual url const &root_directory() const = 0;
    [[nodiscard]] virtual url editing_file() const = 0;
    [[nodiscard]] virtual url playing_directory() const = 0;
};

struct file_importer_for_project {
    virtual ~file_importer_for_project() = default;

    virtual void import(file_importing_context &&) = 0;
    virtual void cancel(std::string const &) = 0;
};

struct file_loader_for_project {
    virtual ~file_loader_for_project() = default;

    [[nodiscard]] virtual std::optional<file_info> load_file_info(url const &) const = 0;
};

struct player_for_project {
    virtual ~player_for_project() = default;

    virtual void set_playing(bool const) = 0;
    virtual bool is_playing() const = 0;

    void toggle_playing() {
        this->set_playing(!this->is_playing());
    }

    virtual void seek(frame_index_t const) = 0;
    virtual frame_index_t current_frame() const = 0;

    [[nodiscard]] virtual observing::syncable observe_is_playing(std::function<void(bool const &)> &&) = 0;
};

struct project_editor_for_project {
    virtual ~project_editor_for_project() = default;

    [[nodiscard]] virtual std::shared_ptr<file_track_for_project_editor> const &file_track() const = 0;
    [[nodiscard]] virtual std::shared_ptr<marker_pool_for_project_editor> const &marker_pool() const = 0;
};

struct project_editor_maker_for_project {
    virtual ~project_editor_maker_for_project() = default;

    [[nodiscard]] virtual std::shared_ptr<project_editor_for_project> make(url const &, file_info const &) const = 0;
};
}  // namespace yas::ae
