//
//  ae_pasting_modules_presenter.hpp
//

#pragma once

#include <ae-core/app/value_types/ae_project_format.h>
#include <ae-core/global/value_types/ae_common_types.h>
#include <ae-core/global/value_types/ae_project_lifetime_id.h>
#include <ae-core/project_editing/features/ae_pasting_module_content_pool.h>

#include <ae-core/global/value_types/ae_space_range.hpp>
#include <ae-core/global/value_types/ae_track_range.hpp>
#include <optional>

namespace yas::ae {
class display_space;
class display_space_range;
class pasteboard;
class track_selector;
class vertical_scrolling;
class space_range;

struct pasting_modules_presenter final {
    [[nodiscard]] static std::shared_ptr<pasting_modules_presenter> make_shared(project_lifetime_id const &);

    pasting_modules_presenter(project_format const &, std::shared_ptr<pasteboard> const &,
                              std::shared_ptr<display_space> const &, std::shared_ptr<display_space_range> const &,
                              std::shared_ptr<pasting_module_content_pool> const &,
                              std::shared_ptr<vertical_scrolling> const &);

    [[nodiscard]] std::vector<std::optional<pasting_module_content>> const &contents() const;
    [[nodiscard]] observing::syncable observe_contents(
        std::function<void(pasting_module_content_pool_event const &)> &&);

    [[nodiscard]] double y_offset() const;

    void update_if_needed();

   private:
    project_format const _project_format;
    std::weak_ptr<pasteboard> const _pasteboard;
    std::weak_ptr<display_space> const _display_space;
    std::weak_ptr<display_space_range> const _display_space_range;
    std::weak_ptr<pasting_module_content_pool> const _content_pool;
    std::weak_ptr<vertical_scrolling> const _scrolling;
    observing::canceller_pool _canceller_pool;

    std::optional<frame_index_t> _last_frame = std::nullopt;
    std::optional<space_range> _last_space_range = std::nullopt;

    pasting_modules_presenter(pasting_modules_presenter const &) = delete;
    pasting_modules_presenter(pasting_modules_presenter &&) = delete;
    pasting_modules_presenter &operator=(pasting_modules_presenter const &) = delete;
    pasting_modules_presenter &operator=(pasting_modules_presenter &&) = delete;

    std::optional<ae::space_range> _space_range() const;

    /// 全ての要素を更新する
    /// - Parameters:
    ///   - force_updating: 表示位置が変わっていなくても強制的に更新する
    ///   - force_replacing: 要素が変わっていなくても強制的に更新する
    void _update_all_contents(bool const force_updating, bool const force_replacing);
};
}  // namespace yas::ae
