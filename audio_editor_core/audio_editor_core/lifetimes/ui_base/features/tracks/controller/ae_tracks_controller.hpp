//
//  ae_tracks_controller.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_project_lifetime_id.h>
#include <audio_editor_core/ae_track_content_pool.h>
#include <ui/yas_ui_types.h>

#include <set>

namespace yas::ae {
class project_action_sender;
class range_selector;
class deselector;

struct tracks_controller final {
    [[nodiscard]] static std::shared_ptr<tracks_controller> make_shared(project_lifetime_id const &);

    tracks_controller(std::shared_ptr<project_action_sender> const &, std::shared_ptr<track_content_pool> const &,
                      std::shared_ptr<range_selector> const &, std::shared_ptr<deselector> const &);

    void deselect_all();
    void begin_range_selection(ui::point const &);

    void begin_selection();
    void select(std::vector<std::size_t> const &);
    void end_selection();
    void toggle_selection(std::size_t const);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;
    std::weak_ptr<track_content_pool> const _content_pool;
    std::weak_ptr<range_selector> const _range_selector;
    std::weak_ptr<deselector> const _deselector;

    tracks_controller(tracks_controller const &) = delete;
    tracks_controller(tracks_controller &&) = delete;
    tracks_controller &operator=(tracks_controller const &) = delete;
    tracks_controller &operator=(tracks_controller &&) = delete;

    std::optional<track_index_t> _track_index(std::size_t const);
    std::set<track_index_t> _track_indices(std::vector<std::size_t> const &);
};
}  // namespace yas::ae
