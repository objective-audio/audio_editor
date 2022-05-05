//
//  ae_time_editor_closer.h
//

#pragma once

#include <cpp_utils/yas_identifier.h>

#include <memory>
#include <optional>

namespace yas::ae {
class time_editor;
class time_editor_state;
class time_editor_level_pool;
class responder_stack;
class timing;
class player;

struct time_editor_closer final {
    [[nodiscard]] static std::shared_ptr<time_editor_closer> make_shared(std::string const &project_id,
                                                                         identifier const level_instance_id,
                                                                         std::shared_ptr<time_editor> const &);

    [[nodiscard]] static std::shared_ptr<time_editor_closer> make_shared(
        identifier const level_instance_id, std::shared_ptr<time_editor> const &,
        std::shared_ptr<time_editor_level_pool> const &, std::shared_ptr<responder_stack> const &,
        std::shared_ptr<timing> const &, std::shared_ptr<player> const &);

    void finish();
    void cancel();

    bool can_finish();
    bool can_cancel();

   private:
    identifier const _level_instance_id;

    struct dependencies {
        std::weak_ptr<time_editor> editor;
        std::weak_ptr<time_editor_level_pool> level_pool;
        std::weak_ptr<responder_stack> responder_stack;
        std::weak_ptr<timing> timing;
        std::weak_ptr<player> player;
    };

    std::optional<dependencies> _dependencies;

    time_editor_closer(identifier const level_instance_id, std::shared_ptr<time_editor> const &,
                       std::shared_ptr<time_editor_level_pool> const &, std::shared_ptr<responder_stack> const &,
                       std::shared_ptr<timing> const &, std::shared_ptr<player> const &);

    void _finalize();
};
}  // namespace yas::ae
