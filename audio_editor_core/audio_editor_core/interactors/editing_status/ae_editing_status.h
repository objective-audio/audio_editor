//
//  ae_editing_status.h
//

#pragma once

#include <memory>

namespace yas::ae {
class exporter;

struct editing_status final {
    [[nodiscard]] static std::shared_ptr<editing_status> make_shared(exporter const *);

    editing_status(exporter const *);

    [[nodiscard]] bool can_editing() const;

   private:
    exporter const *const _exporter;
};
}  // namespace yas::ae
