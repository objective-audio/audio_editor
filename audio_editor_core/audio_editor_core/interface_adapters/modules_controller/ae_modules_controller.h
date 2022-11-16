//
//  ae_modules_controller.h
//

#pragma once

#include <audio_editor_core/ae_module_content_pool.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class project_action_sender;

struct modules_controller final {
    [[nodiscard]] static std::shared_ptr<modules_controller> make_shared(window_lifetime_id const &);
    modules_controller(std::shared_ptr<project_action_sender> const &, std::shared_ptr<module_content_pool> const &);

    void select_module_at(std::size_t const);
    void toggle_module_selection_at(std::size_t const);
    void begin_module_renaming_at(std::size_t const);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;
    std::weak_ptr<module_content_pool> const _content_pool;

    std::optional<file_module_index> index_at(std::size_t const) const;
};
}  // namespace yas::ae