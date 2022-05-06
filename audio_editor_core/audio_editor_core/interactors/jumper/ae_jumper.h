//
//  ae_jumper.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <memory>
#include <optional>

namespace yas::ae {
class player;
class file_track;
class marker_pool;
class edge_editor;

struct jumper final {
    [[nodiscard]] static std::shared_ptr<jumper> make_shared(std::string const &project_id,
                                                             std::shared_ptr<file_track> const &,
                                                             std::shared_ptr<marker_pool> const &,
                                                             std::shared_ptr<edge_editor> const &);
    [[nodiscard]] static std::shared_ptr<jumper> make_shared(std::shared_ptr<player> const &,
                                                             std::shared_ptr<file_track> const &,
                                                             std::shared_ptr<marker_pool> const &,
                                                             std::shared_ptr<edge_editor> const &);

    [[nodiscard]] bool can_jump_to_previous_edge() const;
    [[nodiscard]] bool can_jump_to_next_edge() const;
    [[nodiscard]] bool can_jump_to_beginnig() const;
    [[nodiscard]] bool can_jump_to_end() const;
    void jump_to_previous_edge();
    void jump_to_next_edge();
    void jump_to_beginning();
    void jump_to_end();

   private:
    std::weak_ptr<player> const _player;
    std::weak_ptr<file_track> const _file_track;
    std::weak_ptr<marker_pool> const _marker_pool;
    std::weak_ptr<edge_editor> const _edge_editor;

    jumper(std::shared_ptr<player> const &, std::shared_ptr<file_track> const &, std::shared_ptr<marker_pool> const &,
           std::shared_ptr<edge_editor> const &);

    std::optional<frame_index_t> _previous_jumpable_frame() const;
    std::optional<frame_index_t> _next_jumpable_frame() const;
    std::optional<frame_index_t> _first_edge() const;
    std::optional<frame_index_t> _last_edge() const;
};
}  // namespace yas::ae
