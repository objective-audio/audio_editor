//
//  ae_marker_pool.h
//

#pragma once

#include <ae-core/project_editing/features/ae_jumper_dependency.h>
#include <ae-core/project_editing/features/ae_marker_pool_dependencies.h>
#include <ae-core/project_editing/value_types/ae_marker_pool_types.h>

#include <observing/umbrella.hpp>

namespace yas::ae {
class database;

struct marker_pool final : jumpable_on_jumper {
    marker_pool(database_for_marker_pool *);

    void revert(std::vector<marker_object> &&);
    std::optional<marker_index> insert(frame_index_t const, std::string const &name = "");
    void update(marker_index const, marker_object const &);
    void erase(marker_index const &);
    void erase(frame_index_t const);
    void erase(time::range const);

    void move(marker_index const &, frame_index_t const new_frame);
    void move_offset_from(frame_index_t const from, frame_index_t const offset);

    [[nodiscard]] marker_map_t const &markers() const;
    [[nodiscard]] std::optional<marker_object> marker_at(std::size_t const) const;
    [[nodiscard]] std::optional<marker_object> marker_for_index(marker_index const &) const;
    [[nodiscard]] std::vector<marker_object> markers_for_frame(frame_index_t const &) const;
    [[nodiscard]] std::optional<marker_object> marker_for_id(object_id const &) const;
    [[nodiscard]] bool marker_exists_for_frame(frame_index_t const) const;

    [[nodiscard]] std::optional<frame_index_t> next_jumpable_frame(frame_index_t const) const override;
    [[nodiscard]] std::optional<frame_index_t> previous_jumpable_frame(frame_index_t const) const override;

    [[nodiscard]] observing::syncable observe_event(std::function<void(marker_pool_event const &)> &&);

   private:
    database_for_marker_pool *_database;
    observing::map::holder_ptr<marker_index, marker_object> const _markers;
    observing::fetcher_ptr<marker_pool_event> _fetcher;
    observing::canceller_pool _pool;

    marker_pool(marker_pool const &) = delete;
    marker_pool(marker_pool &&) = delete;
    marker_pool &operator=(marker_pool const &) = delete;
    marker_pool &operator=(marker_pool &&) = delete;
};
}  // namespace yas::ae
