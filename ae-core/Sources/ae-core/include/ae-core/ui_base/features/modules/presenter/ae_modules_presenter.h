//
//  ae_modules_presenter.h
//

#pragma once

#include <ae-core/app/value_types/ae_project_format.h>
#include <ae-core/global/value_types/ae_project_lifetime_id.h>
#include <ae-core/project_editing/features/ae_module_content_pool.h>
#include <ae-core/ui_base/features/modules/presenter/ae_modules_presenter_dependency.h>
#include <ui/common/yas_ui_types.h>

#include <ae-core/global/value_types/ae_space_range.hpp>
#include <ae-core/global/value_types/ae_track_range.hpp>
#include <ae-core/project_editing/features/ae_selected_module_pool.hpp>
#include <ae-core/project_editing/value_types/ae_module_index.hpp>
#include <ae-core/project_editing/value_types/ae_range_selection.hpp>

namespace yas::ae {
class display_space;
class display_space_range;
class player;
class range_selector;
class module_pool;

struct modules_presenter final {
    [[nodiscard]] static std::shared_ptr<modules_presenter> make_shared(project_lifetime_id const &);

    modules_presenter(project_format const &, std::shared_ptr<player> const &, std::shared_ptr<module_pool> const &,
                      std::shared_ptr<selected_module_pool> const &, std::shared_ptr<display_space> const &,
                      std::shared_ptr<display_space_range> const &, std::shared_ptr<module_content_pool> const &,
                      std::shared_ptr<range_selector> const &);

    [[nodiscard]] std::vector<std::optional<module_content>> const &contents() const;
    [[nodiscard]] observing::syncable observe_contents(std::function<void(module_content_pool_event const &)> &&);
    [[nodiscard]] observing::syncable observe_range(std::function<void(range_selection const &)> &&);

    std::string name_for_index(module_index const &);

    void update_if_needed();

   private:
    project_format const _project_format;
    std::weak_ptr<player> const _player;
    std::weak_ptr<module_pool> const _module_pool;
    std::weak_ptr<selected_module_pool> const _selected_pool;
    std::weak_ptr<display_space> const _display_space;
    std::weak_ptr<display_space_range> const _display_space_range;
    std::weak_ptr<module_content_pool> const _content_pool;
    std::weak_ptr<range_selector> const _range_selector;
    observing::canceller_pool _canceller_pool;

    std::optional<frame_index_t> _last_frame = std::nullopt;
    std::optional<space_range> _last_space_range = std::nullopt;

    modules_presenter(modules_presenter const &) = delete;
    modules_presenter(modules_presenter &&) = delete;
    modules_presenter &operator=(modules_presenter const &) = delete;
    modules_presenter &operator=(modules_presenter &&) = delete;

    [[nodiscard]] std::optional<ae::space_range> _space_range() const;
    void _insert_content(module_object const &);
    void _erase_content(object_id const &);
    void _replace_contents(selected_module_set const &);
    void _update_all_contents(bool const force_updating, bool const force_replacing);
};
}  // namespace yas::ae
