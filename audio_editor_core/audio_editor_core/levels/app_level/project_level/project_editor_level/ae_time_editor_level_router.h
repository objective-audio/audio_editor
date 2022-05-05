//
//  ae_time_editor_level_router.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class time_editor_level;
class number_components;

struct time_editor_level_router final {
    [[nodiscard]] static std::shared_ptr<time_editor_level_router> make_shared(std::string const &identifier);

    void add_level(number_components const &, std::optional<std::size_t> const unit_idx);
    void remove_level();

    [[nodiscard]] std::shared_ptr<time_editor_level> const &level();

    [[nodiscard]] observing::syncable observe_level(std::function<void(std::shared_ptr<time_editor_level> const &)> &&);

   private:
    std::weak_ptr<time_editor_level_router> _weak_pool;

    std::string const _identifier;

    observing::value::holder_ptr<std::shared_ptr<time_editor_level>> const _level;

    time_editor_level_router(std::string const &identifier);
};
}  // namespace yas::ae
