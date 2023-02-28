//
//  ae_modules_controller.h
//

#pragma once

#include <audio_editor_core/ae_module_content_pool.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class project_action_sender;
class range_selector;

struct modules_controller final {
    [[nodiscard]] static std::shared_ptr<modules_controller> make_shared(window_lifetime_id const &);
    modules_controller(std::shared_ptr<project_action_sender> const &, std::shared_ptr<module_content_pool> const &,
                       std::shared_ptr<range_selector> const &);

    void select(std::size_t const);
    void toggle_selection(std::size_t const);
    void begin_renaming(std::size_t const);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;
    std::weak_ptr<module_content_pool> const _content_pool;
    std::weak_ptr<range_selector> const _range_selector;

    modules_controller(modules_controller const &) = delete;
    modules_controller(modules_controller &&) = delete;
    modules_controller &operator=(modules_controller const &) = delete;
    modules_controller &operator=(modules_controller &&) = delete;

    [[nodiscard]] std::optional<module_index> _module_index(std::size_t const) const;
    [[nodiscard]] std::vector<module_index> _module_indices(std::vector<std::size_t> const &) const;
};
}  // namespace yas::ae
