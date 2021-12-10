//
//  ae_resource_pool.h
//

#pragma once

#include <audio_editor_core/ae_identifier.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
enum class resource_pool_event_type {
    fetched,
    replaced,
    updated,
};

template <typename Resource>
struct resource_pool_event {
    resource_pool_event_type type;
    std::vector<std::optional<Resource>> const &locations;   // fetched, replaced
    std::vector<std::pair<std::size_t, Resource>> inserted;  // updated
    std::vector<std::pair<std::size_t, Resource>> erased;    // updated
};

template <typename Resource>
struct resource_pool final {
    [[nodiscard]] static std::shared_ptr<resource_pool<Resource>> make_shared();

    void replace_all(std::vector<Resource> const &);
    void update_all(std::vector<Resource> const &);
    void erase(identifier const &);
    void insert(Resource const &);

    std::vector<std::optional<Resource>> const &locations() const;

    [[nodiscard]] observing::syncable observe_event(std::function<void(resource_pool_event<Resource> const &)> &&);

   private:
    std::vector<std::optional<Resource>> _locations;
    observing::fetcher_ptr<resource_pool_event<Resource>> _fetcher;

    resource_pool();
};
}  // namespace yas::ae

#include <audio_editor_core/ae_resource_pool_private.h>
