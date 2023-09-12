//
//  ae_module_pool.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_jumper_dependency.h>
#include <audio_editor_core/ae_module_pool_dependencies.h>
#include <audio_editor_core/ae_module_pool_types.h>

#include <observing/yas_observing_umbrella.hpp>
#include <set>

namespace yas::ae {
struct module_pool final : jumpable_on_jumper {
    module_pool(database_for_module_pool *);

    [[nodiscard]] module_pool_module_map_t const &modules() const;

    void revert_modules_and_notify(std::vector<module_object> &&);
    std::optional<module_index> insert_module_and_notify(module const &);
    void erase_module_and_notify(module_index const &);
    void set_module_name_and_notify(module_index const &, std::string const &);

    [[nodiscard]] std::optional<time::range> total_range() const;
    [[nodiscard]] std::optional<module_object> module_at(module_index const &) const;
    [[nodiscard]] module_pool_module_map_t modules_at(std::set<track_index_t> const &, frame_index_t const) const;
    [[nodiscard]] bool has_modules_at(std::set<track_index_t> const &, frame_index_t const) const;
    [[nodiscard]] module_pool_module_map_t splittable_modules_at(std::set<track_index_t> const &,
                                                                 frame_index_t const) const;
    [[nodiscard]] std::optional<frame_index_t> first_frame() const;
    [[nodiscard]] std::optional<frame_index_t> last_next_frame() const;
    [[nodiscard]] std::optional<frame_index_t> next_jumpable_frame(frame_index_t const) const override;
    [[nodiscard]] std::optional<frame_index_t> previous_jumpable_frame(frame_index_t const) const override;

    void split_at(std::set<track_index_t> const &, frame_index_t const);
    void erase_at(std::set<track_index_t> const &, frame_index_t const);
    void drop_head_at(std::set<track_index_t> const &, frame_index_t const);
    void drop_tail_at(std::set<track_index_t> const &, frame_index_t const);
    void overwrite_module(module const &);

    [[nodiscard]] observing::syncable observe_event(std::function<void(module_pool_event const &)> &&);

   private:
    database_for_module_pool *_database;

    module_pool_module_map_t _modules;

    observing::fetcher_ptr<module_pool_event> _event_fetcher;

    module_pool(module_pool const &) = delete;
    module_pool(module_pool &&) = delete;
    module_pool &operator=(module_pool const &) = delete;
    module_pool &operator=(module_pool &&) = delete;

    void _move_modules_after(frame_index_t const frame, frame_index_t const offset);
};
}  // namespace yas::ae
