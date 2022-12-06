//
//  ae_selected_file_module_pool.hpp
//

#pragma once

#include <observing/yas_observing_umbrella.h>

#include <audio_editor_core/ae_selected_file_module.hpp>
#include <map>

namespace yas::ae {
struct selected_file_module_pool final {
    using module_map = std::map<file_module_index, selected_file_module_object>;

    enum class event_type {
        fetched,
        inserted,
        erased,
    };

    struct event final {
        event_type type;
        module_map modules;  // inserted, erased
    };

    selected_file_module_pool();

    [[nodiscard]] module_map const &modules() const;

    bool contains(file_module_index const &) const;

    void toggle_module(selected_file_module_object const &);
    void insert_module(selected_file_module_object const &);
    void erase_module(file_module_index const &);

    [[nodiscard]] bool can_clear() const;
    void clear();

    [[nodiscard]] observing::syncable observe_event(std::function<void(event const &)> &&);

   private:
    module_map _modules;

    observing::fetcher_ptr<event> _event_fetcher;

    selected_file_module_pool(selected_file_module_pool const &) = delete;
    selected_file_module_pool(selected_file_module_pool &&) = delete;
    selected_file_module_pool &operator=(selected_file_module_pool const &) = delete;
    selected_file_module_pool &operator=(selected_file_module_pool &&) = delete;
};
}  // namespace yas::ae
