//
//  ae_module_location_pool.h
//

#pragma once

#include <audio_editor_core/ae_module_location_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct module_location_pool final {
    [[nodiscard]] static std::shared_ptr<module_location_pool> make_shared();

    void replace_all(std::vector<module_location> const &);
    void update_all(std::vector<module_location> const &);
    void erase(identifier const &);
    void insert(module_location const &);

    std::vector<std::optional<module_location>> const &locations() const;

    [[nodiscard]] observing::syncable observe_event(std::function<void(module_location_pool_event const &)> &&);

   private:
    std::vector<std::optional<module_location>> _locations;
    observing::fetcher_ptr<module_location_pool_event> _fetcher;

    module_location_pool();
};
}  // namespace yas::ae
