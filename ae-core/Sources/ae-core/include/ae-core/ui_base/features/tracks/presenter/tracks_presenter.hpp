//
//  tracks_presenter.hpp
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ae-core/project_editing/features/track_content_pool.h>

#include <ae-core/global/value_types/space_range.hpp>
#include <ae-core/project_editing/features/selected_track_pool.hpp>
#include <ae-core/project_editing/value_types/range_selection.hpp>

namespace yas::ae {
class display_space;
class display_space_range;
class range_selector;

struct tracks_presenter final {
    [[nodiscard]] static std::shared_ptr<tracks_presenter> make_shared(project_lifetime_id const &);

    tracks_presenter(project_lifetime_id const &, std::shared_ptr<track_content_pool> const &,
                     std::shared_ptr<selected_track_pool> const &, std::shared_ptr<display_space> const &,
                     std::shared_ptr<display_space_range> const &, std::shared_ptr<range_selector> const &);

    [[nodiscard]] std::vector<std::optional<track_content>> const &contents() const;
    [[nodiscard]] observing::syncable observe_contents(std::function<void(track_content_pool_event const &)> &&);
    [[nodiscard]] observing::syncable observe_range(std::function<void(range_selection const &)> &&);

    void update_if_needed();

   private:
    std::weak_ptr<track_content_pool> const _content_pool;
    std::weak_ptr<selected_track_pool> const _selected_pool;
    std::weak_ptr<display_space> const _display_space;
    std::weak_ptr<display_space_range> const _display_space_range;
    std::weak_ptr<range_selector> const _range_selector;
    observing::canceller_pool _canceller_pool;

    std::optional<space_range> _last_space_range = std::nullopt;

    std::optional<ae::space_range> _space_range() const;

    void _replace_contents(selected_track_set const &);

    enum class update_type {
        replace,            // 全て更新。scaleが変わったので強制的に全て置き換える
        update,             // 全て更新。位置は変わらないので変更があったところだけ更新
        update_if_changed,  // 前回の更新から変更などがあったら更新
    };

    void _update_all_contents(update_type const type);
};
}  // namespace yas::ae
