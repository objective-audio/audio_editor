//
//  ae_modules_controller.h
//

#pragma once

#include <audio_editor_core/ae_module_location_pool.h>

namespace yas::ae {
class action_controller;

struct modules_controller final {
    static std::shared_ptr<modules_controller> make_shared(std::string const &project_id,
                                                           std::shared_ptr<module_location_pool> const &);

    void select_module_at(std::size_t const);

   private:
    std::shared_ptr<action_controller> const _action_controller;
    std::shared_ptr<module_location_pool> const _location_pool;

    modules_controller(std::shared_ptr<action_controller> const &, std::shared_ptr<module_location_pool> const &);
};
}  // namespace yas::ae
