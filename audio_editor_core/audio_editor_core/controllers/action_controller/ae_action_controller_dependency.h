//
//  ae_action_controller_dependency.h
//

#pragma once

#include <cstddef>
#include <memory>

namespace yas::ae {
struct project_editor_for_action_controller {
    virtual ~project_editor_for_action_controller() = default;

    virtual void set_playing(bool const) = 0;
    [[nodiscard]] virtual bool is_playing() const = 0;

    void toggle_playing() {
        this->set_playing(!this->is_playing());
    }

    virtual void nudge_previous() = 0;
    virtual void nudge_next() = 0;
    virtual void jump_to_previous_edge() = 0;
    virtual void jump_to_next_edge() = 0;
    virtual void split() = 0;
    virtual void drop_head_and_offset() = 0;
    virtual void drop_tail_and_offset() = 0;
    virtual void erase_and_offset() = 0;
    virtual void insert_marker() = 0;
    virtual void return_to_zero() = 0;
    virtual void go_to_marker(std::size_t const) = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};
}  // namespace yas::ae
