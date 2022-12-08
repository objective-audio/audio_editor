//
//  ae_time_editor_opener.h
//

#pragma once

#include <memory>
#include <optional>

namespace yas::ae {
class player;
class timing;
class project_modal_lifecycle;

struct time_editor_opener final {
    time_editor_opener(player const *, timing const *, project_modal_lifecycle *);

    [[nodiscard]] bool can_begin_time_editing() const;
    void begin_time_editing();

   private:
    player const *const _player;
    timing const *const _timing;
    project_modal_lifecycle *const _lifecycle;

    time_editor_opener(time_editor_opener const &) = delete;
    time_editor_opener(time_editor_opener &&) = delete;
    time_editor_opener &operator=(time_editor_opener const &) = delete;
    time_editor_opener &operator=(time_editor_opener &&) = delete;
};
}  // namespace yas::ae
