//
//  ae_ui_markers.h
//

#pragma once

#include <audio_editor_core/ae_marker_content_pool.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class markers_presenter;
class display_space;
class ui_marker_element;
class ui_mesh_data;

struct ui_markers final {
    [[nodiscard]] static std::shared_ptr<ui_markers> make_shared(window_lifetime_id const &,
                                                                 std::shared_ptr<ui::node> const &);

    ui_markers(window_lifetime_id const &, std::shared_ptr<markers_presenter> const &,
               std::shared_ptr<ui::standard> const &, ui::node *);

   private:
    window_lifetime_id const _window_lifetime_id;
    ui::node *const _node;
    std::shared_ptr<markers_presenter> const _presenter;

    std::shared_ptr<ui::layout_value_guide> const _top_guide;
    std::vector<std::shared_ptr<ui_marker_element>> _elements;

    observing::canceller_pool _pool;

    void _set_count(std::size_t const);
    void _replace_contents(std::vector<std::optional<marker_content>> const &);
    void _update_contents(std::size_t const count, std::vector<std::pair<std::size_t, marker_content>> const &erased,
                          std::vector<std::pair<std::size_t, marker_content>> const &inserted,
                          std::vector<std::pair<std::size_t, marker_content>> const &replaced);
};
}  // namespace yas::ae
