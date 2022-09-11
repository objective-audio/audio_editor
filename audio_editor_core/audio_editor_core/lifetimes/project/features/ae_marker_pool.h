//
//  ae_marker_pool.h
//

#pragma once

#include <audio_editor_core/ae_jumper_dependency.h>
#include <audio_editor_core/ae_marker_pool_dependencies.h>
#include <audio_editor_core/ae_marker_pool_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class database;

struct marker_pool final : jumpable_on_jumper {
    [[nodiscard]] static std::shared_ptr<marker_pool> make_shared(database_for_marker_pool *);

    marker_pool(database_for_marker_pool *);

    void revert_markers(std::vector<marker> &&);
    void insert_marker(frame_index_t const, std::string const &name = "");
    void update_marker(frame_index_t const, marker const &);
    void erase_at(frame_index_t const);
    void erase_marker(marker const &);
    void erase_range(time::range const);

    void move_at(frame_index_t const frame, frame_index_t const new_frame);
    void move_offset_from(frame_index_t const from, frame_index_t const offset);

    [[nodiscard]] marker_map_t const &markers() const;
    [[nodiscard]] std::optional<marker> marker_at(std::size_t const) const;
    [[nodiscard]] std::optional<marker> marker_for_frame(frame_index_t const) const;
    [[nodiscard]] std::optional<marker> marker_for_id(object_id const &) const;

    [[nodiscard]] std::optional<frame_index_t> next_jumpable_frame(frame_index_t const) const override;
    [[nodiscard]] std::optional<frame_index_t> previous_jumpable_frame(frame_index_t const) const override;

    [[nodiscard]] observing::syncable observe_event(std::function<void(marker_pool_event const &)> &&);

   private:
    database_for_marker_pool *_database;
    observing::map::holder_ptr<frame_index_t, marker> const _markers;
    observing::fetcher_ptr<marker_pool_event> _fetcher;
    observing::canceller_pool _pool;
};
}  // namespace yas::ae
