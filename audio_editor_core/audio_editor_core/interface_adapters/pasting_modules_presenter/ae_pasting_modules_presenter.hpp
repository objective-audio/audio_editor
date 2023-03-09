//
//  ae_pasting_modules_presenter.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_pasting_module_content_pool.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

#include <optional>

namespace yas::ae {
class display_space;
class pasteboard;

struct pasting_modules_presenter final {
    [[nodiscard]] static std::shared_ptr<pasting_modules_presenter> make_shared(window_lifetime_id const &);

    pasting_modules_presenter(project_format const &, std::shared_ptr<pasteboard> const &,
                              std::shared_ptr<display_space> const &,
                              std::shared_ptr<pasting_module_content_pool> const &);

    [[nodiscard]] std::vector<std::optional<pasting_module_content>> const &contents() const;
    [[nodiscard]] observing::syncable observe_contents(
        std::function<void(pasting_module_content_pool_event const &)> &&);

    void update_if_needed();

   private:
    project_format const _project_format;
    std::weak_ptr<pasteboard> const _pasteboard;
    std::weak_ptr<display_space> const _display_space;
    std::weak_ptr<pasting_module_content_pool> const _content_pool;
    observing::canceller_pool _canceller_pool;

    std::optional<frame_index_t> _last_frame = std::nullopt;
    std::optional<time::range> _last_space_range = std::nullopt;

    pasting_modules_presenter(pasting_modules_presenter const &) = delete;
    pasting_modules_presenter(pasting_modules_presenter &&) = delete;
    pasting_modules_presenter &operator=(pasting_modules_presenter const &) = delete;
    pasting_modules_presenter &operator=(pasting_modules_presenter &&) = delete;

    std::optional<time::range> _space_range() const;

    /// 全ての要素を更新する
    /// - Parameters:
    ///   - force_updating: 表示位置が変わっていなくても強制的に更新する
    ///   - force_replacing: 要素が変わっていなくても強制的に更新する
    void _update_all_contents(bool const force_updating, bool const force_replacing);
};
}  // namespace yas::ae
