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
    [[nodiscard]] static std::shared_ptr<time_editor_opener> make_shared(player const *, timing const *,
                                                                         project_modal_lifecycle *);

    time_editor_opener(player const *, timing const *, project_modal_lifecycle *);

    bool can_begin_time_editing() const;
    void begin_time_editing();

   private:
    player const *const _player;
    timing const *const _timing;
    project_modal_lifecycle *const _lifecycle;
};
}  // namespace yas::ae
