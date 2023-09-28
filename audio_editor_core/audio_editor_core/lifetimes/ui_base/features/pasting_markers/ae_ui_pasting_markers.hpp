//
//  ae_ui_pasting_markers.hpp
//

#pragma once

#include <audio_editor_core/ae_pasting_marker_content_pool.h>
#include <audio_editor_core/ae_project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class pasting_markers_presenter;
class ui_pasting_marker_element;

struct ui_pasting_markers final {
    static std::shared_ptr<ui_pasting_markers> make_shared(project_lifetime_id const &,
                                                           std::shared_ptr<ui::node> const &);
    ui_pasting_markers(project_lifetime_id const &, std::shared_ptr<pasting_markers_presenter> const &,
                       std::shared_ptr<ui::standard> const &, ui::node *);

   private:
    project_lifetime_id const _project_lifetime_id;
    std::shared_ptr<pasting_markers_presenter> _presenter;
    ui::node *const _node;

    std::vector<std::shared_ptr<ui_pasting_marker_element>> _elements;

    observing::canceller_pool _pool;

    ui_pasting_markers(ui_pasting_markers const &) = delete;
    ui_pasting_markers(ui_pasting_markers &&) = delete;
    ui_pasting_markers &operator=(ui_pasting_markers const &) = delete;
    ui_pasting_markers &operator=(ui_pasting_markers &&) = delete;

    void _replace();
    void _update(std::set<std::size_t> const &inserted_indices, std::set<std::size_t> const &replaced_indices,
                 std::map<std::size_t, pasting_marker_content> const &erased);
    void _set_count(std::size_t const);
};
}  // namespace yas::ae
