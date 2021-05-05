//
//  ae_timeline_editor_dependency.h
//

#pragma once

#include <memory>

namespace yas::proc {
class timeline;
}

namespace yas::ae {
struct timeline_player_interface {
    virtual ~timeline_player_interface() = default;

    virtual void set_timeline(std::shared_ptr<proc::timeline> const &) = 0;
};
}  // namespace yas::ae
