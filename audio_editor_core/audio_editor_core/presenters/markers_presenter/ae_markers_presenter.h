//
//  ae_markers_presenter.h
//

#pragma once

#include <audio_editor_core/ae_marker_location_pool.h>
#include <audio_editor_core/ae_markers_presenter_dependency.h>

namespace yas::ae {
class display_space;

struct markers_presenter final {
    [[nodiscard]] static std::shared_ptr<markers_presenter> make_shared(std::string const &project_id,
                                                                        std::shared_ptr<display_space> const &);
    [[nodiscard]] static std::shared_ptr<markers_presenter> make_shared(
        std::shared_ptr<project_editor_for_markers_presenter> const &, std::shared_ptr<display_space> const &);

    [[nodiscard]] std::vector<std::optional<marker_location>> locations() const;
    [[nodiscard]] observing::syncable observe_locations(std::function<void(marker_location_pool_event const &)> &&);

    void update_if_needed();

   private:
    std::weak_ptr<project_editor_for_markers_presenter> _project_editor;
    std::shared_ptr<display_space> const _display_space;
    std::shared_ptr<marker_location_pool> const _location_pool;
    observing::canceller_pool _canceller_pool;

    std::optional<frame_index_t> _last_frame = std::nullopt;
    std::optional<time::range> _last_space_range = std::nullopt;

    markers_presenter(std::shared_ptr<project_editor_for_markers_presenter> const &,
                      std::shared_ptr<display_space> const &);

    markers_presenter(markers_presenter const &) = delete;
    markers_presenter(markers_presenter &&) = delete;
    markers_presenter &operator=(markers_presenter const &) = delete;
    markers_presenter &operator=(markers_presenter &&) = delete;

    [[nodiscard]] std::optional<time::range> _space_range() const;

    enum class update_type {
        replace,            // 全て更新。scaleが変わったので強制的に全て置き換える
        update,             // 全て更新。位置は変わらないので変更があったところだけ更新
        update_if_changed,  // 前回の更新から変更などがあったら更新
    };

    void _update_all_locations(update_type const type);
};
}  // namespace yas::ae
