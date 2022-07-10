//
//  ae_modules_controller.h
//

#pragma once

#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class project_action_sender;

struct modules_controller final {
    [[nodiscard]] static std::shared_ptr<modules_controller> make_shared(window_lifetime_id const &,
                                                                         std::shared_ptr<module_location_pool> const &);

    void select_module_at(std::size_t const);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;
    std::shared_ptr<module_location_pool> const _location_pool;

    modules_controller(std::shared_ptr<project_action_sender> const &, std::shared_ptr<module_location_pool> const &);
};
}  // namespace yas::ae
