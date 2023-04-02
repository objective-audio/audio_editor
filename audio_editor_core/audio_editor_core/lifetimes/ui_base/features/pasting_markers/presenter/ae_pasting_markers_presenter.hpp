//
//  ae_pasting_markers_presenter.hpp
//

#pragma once

#include <audio_editor_core/ae_pasting_marker_content_pool.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <observing/yas_observing_umbrella.h>

#include <memory>

namespace yas::ae {
class pasteboard;
class display_space;
class display_space_range;

struct pasting_markers_presenter {
    [[nodiscard]] static std::shared_ptr<pasting_markers_presenter> make_shared(window_lifetime_id const &);

    explicit pasting_markers_presenter(project_format const &, std::shared_ptr<pasteboard> const &,
                                       std::shared_ptr<display_space> const &,
                                       std::shared_ptr<display_space_range> const &,
                                       std::shared_ptr<pasting_marker_content_pool> const &);

    [[nodiscard]] std::vector<std::optional<pasting_marker_content>> const &contents() const;
    [[nodiscard]] observing::syncable observe_contents(
        std::function<void(pasting_marker_content_pool_event const &)> &&);

    void update_if_needed();

   private:
    project_format const _project_format;
    std::weak_ptr<pasteboard> const _pasteboard;
    std::weak_ptr<display_space> const _display_space;
    std::weak_ptr<display_space_range> const _display_space_range;
    std::weak_ptr<pasting_marker_content_pool> const _content_pool;
    observing::canceller_pool _canceller_pool;

    std::optional<frame_index_t> _last_frame = std::nullopt;
    std::optional<time::range> _last_space_range = std::nullopt;

    pasting_markers_presenter(pasting_markers_presenter const &) = delete;
    pasting_markers_presenter(pasting_markers_presenter &&) = delete;
    pasting_markers_presenter &operator=(pasting_markers_presenter const &) = delete;
    pasting_markers_presenter &operator=(pasting_markers_presenter &&) = delete;

    std::optional<time::range> _space_range() const;

    /// 全ての要素を更新する
    /// - Parameters:
    ///   - force_updating: 表示位置が変わっていなくても強制的に更新する
    ///   - force_replacing: 要素が変わっていなくても強制的に更新する
    void _update_all_contents(bool const force_updating, bool const force_replacing);
};
}  // namespace yas::ae
