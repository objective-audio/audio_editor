//
//  ae_modules_controller.h
//

#pragma once

#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_project_id.h>

namespace yas::ae {
class action_controller;

struct modules_controller final {
    [[nodiscard]] static std::shared_ptr<modules_controller> make_shared(project_id const &project_id,
                                                                         std::shared_ptr<module_location_pool> const &);

    void select_module_at(std::size_t const);

   private:
    std::shared_ptr<action_controller> const _action_controller;
    std::shared_ptr<module_location_pool> const _location_pool;

    modules_controller(std::shared_ptr<action_controller> const &, std::shared_ptr<module_location_pool> const &);
};
}  // namespace yas::ae
