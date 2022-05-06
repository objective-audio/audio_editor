//
//  ae_nudger.h
//

#pragma once

#include <memory>

namespace yas::ae {
class player;
class nudge_settings;

struct nudger final {
    [[nodiscard]] static std::shared_ptr<nudger> make_shared(std::string const &project_id,
                                                             std::shared_ptr<nudge_settings> const &);
    [[nodiscard]] static std::shared_ptr<nudger> make_shared(std::shared_ptr<player> const &,
                                                             std::shared_ptr<nudge_settings> const &);

    [[nodiscard]] bool can_nudge() const;
    void nudge_previous(uint32_t const offset_count);
    void nudge_next(uint32_t const offset_count);

   private:
    std::weak_ptr<player> const _player;
    std::weak_ptr<nudge_settings> const _settings;

    nudger(std::shared_ptr<player> const &, std::shared_ptr<nudge_settings> const &);
};
}  // namespace yas::ae
