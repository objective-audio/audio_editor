//
//  markers_presenter.h
//

#pragma once

#include <ae-core/app/value_types/project_format.h>
#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ae-core/project_editing/features/marker_content_pool.h>

#include <ae-core/project_editing/features/selected_marker_pool.hpp>
#include <ae-core/project_editing/value_types/range_selection.hpp>

namespace yas::ae {
class display_space;
class display_space_time_range;
class player;
class marker_pool;
class range_selector;
class marker_index;

struct markers_presenter final {
    [[nodiscard]] static std::shared_ptr<markers_presenter> make_shared(project_lifetime_id const &,
                                                                        std::shared_ptr<marker_content_pool> const &);

    markers_presenter(project_format const &, std::shared_ptr<player> const &, std::shared_ptr<marker_pool> const &,
                      std::shared_ptr<selected_marker_pool> const &, std::shared_ptr<display_space> const &,
                      std::shared_ptr<display_space_time_range> const &, std::shared_ptr<marker_content_pool> const &,
                      std::shared_ptr<range_selector> const &);

    [[nodiscard]] std::vector<std::optional<marker_content>> const &contents() const;
    [[nodiscard]] observing::syncable observe_contents(std::function<void(marker_content_pool_event const &)> &&);
    [[nodiscard]] observing::syncable observe_range(std::function<void(range_selection const &)> &&);

    [[nodiscard]] std::optional<marker_index> marker_index_at(std::size_t const content_idx) const;

    void update_if_needed();

   private:
    project_format const _project_format;
    std::weak_ptr<player> const _player;
    std::weak_ptr<marker_pool> const _marker_pool;
    std::weak_ptr<selected_marker_pool> const _selected_marker_pool;
    std::weak_ptr<display_space> const _display_space;
    std::weak_ptr<display_space_time_range> const _display_space_time_range;
    std::shared_ptr<marker_content_pool> const _content_pool;
    std::weak_ptr<range_selector> const _range_selector;
    observing::canceller_pool _canceller_pool;

    std::optional<frame_index_t> _last_frame = std::nullopt;
    std::optional<time::range> _last_space_range = std::nullopt;

    markers_presenter(markers_presenter const &) = delete;
    markers_presenter(markers_presenter &&) = delete;
    markers_presenter &operator=(markers_presenter const &) = delete;
    markers_presenter &operator=(markers_presenter &&) = delete;

    [[nodiscard]] std::optional<time::range> _space_range() const;

    enum class update_type {
        replace,            // 全て更新。scaleが変わったので強制的に全て置き換える
        update,             // 全て更新。位置は変わらないので変更があったところだけ更新
        update_if_changed,  // 前回の更新から画面や再生位置に変更があったら更新
    };

    void _update_all_contents(update_type const type);
    void _insert_content_if_in_space_range(marker_index const &);
    void _replace_or_erase_content_if_in_space_range(marker_index const &inserted_index,
                                                     marker_index const &erased_index);
    void _replace_contents_if_in_space_range(selected_marker_set const &changed);

    bool _is_selected(marker_index const &) const;
};
}  // namespace yas::ae
