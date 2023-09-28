//
//  ae_ui_markers.h
//

#pragma once

#include <audio_editor_core/ae_marker_content_pool.h>
#include <audio_editor_core/ae_project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class markers_presenter;
class markers_controller;
class ui_marker_element;

struct ui_markers final {
    [[nodiscard]] static std::shared_ptr<ui_markers> make_shared(project_lifetime_id const &,
                                                                 std::shared_ptr<ui::node> const &);

    ui_markers(project_lifetime_id const &, std::shared_ptr<markers_presenter> const &,
               std::shared_ptr<markers_controller> const &, std::shared_ptr<ui::standard> const &, ui::node *);

   private:
    project_lifetime_id const _project_lifetime_id;
    ui::node *const _node;
    std::shared_ptr<markers_presenter> const _presenter;
    std::shared_ptr<markers_controller> const _controller;

    std::vector<std::shared_ptr<ui_marker_element>> _elements;

    observing::canceller_pool _pool;

    ui_markers(ui_markers const &) = delete;
    ui_markers(ui_markers &&) = delete;
    ui_markers &operator=(ui_markers const &) = delete;
    ui_markers &operator=(ui_markers &&) = delete;

    void _replace_elements(std::vector<std::optional<marker_content>> const &);
    void _update_elements(std::size_t const count, std::map<std::size_t, marker_content> const &erased,
                          std::set<std::size_t> const &inserted_indices, std::set<std::size_t> const &replaced_indices);
    void _remake_elements_if_needed(std::size_t const);
};
}  // namespace yas::ae
