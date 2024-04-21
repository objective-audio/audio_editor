//
//  ae_time_editing_opener.h
//

#pragma once

#include <memory>
#include <optional>

namespace yas::ae {
class player;
class timing;
class project_modal_lifecycle;

struct time_editing_opener final {
    time_editing_opener(player const *, timing const *, project_modal_lifecycle *);

    [[nodiscard]] bool can_begin_time_editing() const;
    void begin_time_editing();

   private:
    player const *const _player;
    timing const *const _timing;
    project_modal_lifecycle *const _lifecycle;

    time_editing_opener(time_editing_opener const &) = delete;
    time_editing_opener(time_editing_opener &&) = delete;
    time_editing_opener &operator=(time_editing_opener const &) = delete;
    time_editing_opener &operator=(time_editing_opener &&) = delete;
};
}  // namespace yas::ae
